/**
 * COSC 3250 - Project 9
 * Deletes a file from the directory list, then calls a function to clear the block and write it to disk.
 * @author Lucas Wagner, Jonah Harlan
 * Instructor Professor Brylow
 * TA-BOT:MAILTO [l.wagner@marquette.edu, jonah.harlan@marquette.edu]
 */

/* fileDelete.c - fileDelete */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/*                                                                 */
/* and                                                             */
/*                                                                 */
/*                                                                 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * fileDelete - Delete a file.
 *------------------------------------------------------------------------
 */
devcall fileDelete(int fd)
{
    // TODO: Unlink this file from the master directory index,
    //  and return its space to the free disk block list.
    //  Use the superblock's locks to guarantee mutually exclusive
    //  access to the directory index.

    	struct superblock *psuper = supertab;
	struct dirblock *pdir;
	struct filenode *pfile;
	struct dentry *phw;

	if((NULL == supertab) || (NULL == filetab) || (isbadfd(fd)) || (FILE_FREE == filetab[fd].fn_state)) return SYSERR;
	
	wait(psuper -> sb_dirlock);
	// Need to clear this file's dirblock info and write it to disk
	// Get directory and find the file
	pdir = psuper -> sb_dirlst;
	pfile = &(pdir -> db_fnodes[fd]);
	
	// Return space to free disk block list
	sbFreeBlock(psuper, pfile -> fn_blocknum);
	
	// Reset file info
	pfile -> fn_state = FILE_FREE;
	pfile -> fn_length = 0;
	pfile -> fn_cursor = 0;
	pfile -> fn_blocknum = 0;
	pfile -> fn_name[0] = '\0';
	pfile -> fn_data = NULL;

	// Update disk on new dir
	phw = psuper -> sb_disk;
	int diskfd = phw - devtab;

	seek(diskfd, pdir -> db_blocknum);
	if(write(diskfd, pdir, sizeof(struct dirblock)) == NULL){
		printf("Error writing updated Dir to disk!\n");
		signal(psuper -> sb_dirlock);
		return SYSERR;
	}

	signal(psuper -> sb_dirlock);
	return OK;
}
