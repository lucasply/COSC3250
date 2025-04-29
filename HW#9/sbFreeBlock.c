/**
 * COSC 3250 - Project 9
 * Frees a block and writes it to disk
 * @author Lucas Wagner, Jonah Harlan
 * Instructor Professor Brylow
 * TA-BOT:MAILTO [l.wagner@marquette.edu, jonah.harlan@marquette.edu]
 */

/* sbFreeBlock.c - sbFreeBlock */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/*                                                                 */
/* and                                                             */
/*                                                                 */
/*                                                                 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * sbFreeBlock - Add a block back into the free list of disk blocks.
 *------------------------------------------------------------------------
 */
devcall sbFreeBlock(struct superblock *psuper, int block)
{
    // TODO: Add the block back into the filesystem's list of
    //  free blocks.  Use the superblock's locks to guarantee
    //  mutually exclusive access to the free list, and write
    //  the changed free list segment(s) back to disk.
	
	struct fbcnode *fbc = psuper -> sb_freelst; // Does this only work if the first FBC node is valid?
	struct fbcnode *Newfbc = malloc(sizeof(struct fbcnode));
	struct dentry *phw;
	int diskfd;

	if(psuper == NULL) return SYSERR;

	// This is taken from sbGetBlock
	phw = psuper -> sb_disk;
	diskfd = phw - devtab;
	wait(psuper -> sb_freelock);

	// Get end of freelist
	struct fbcnode *curr = psuper -> sb_freelst;
	int i = 0;
	while(curr -> fbc_next != NULL){
		curr = curr -> fbc_next;
		if(curr == NULL){
			kprintf("While loop is breaking something! \n");
		}
		//kprintf("Curr is at block: %d\n\r", curr -> fbc_blocknum);
	}
	
	// If there are no freenodes, this freed block becomes an FBC node
	//if(psuper -> sb_freelst == NULL){}
		
	// Handling case for if the tail FBC node is full
	if(curr -> fbc_count == FREEBLOCKMAX){
		printf("Creating a new FBC node!\n");
		// Need to set head free block in the head FBC node to be a new FBC node. 
		// Then, set next pointer in final FBC node to point to this new FBC node.
		
		// fbc is the first FBC node, curr is the last FBC node
		// Setting new node accounting info
		
		Newfbc -> fbc_blocknum = block;
		//printf("Newfbc -> blocknum = %d\n", Newfbc -> fbc_blocknum);
		Newfbc -> fbc_count = 0;
		Newfbc -> fbc_next = NULL;	

		kprintf("Head FBC Node: %d, Head block: %d, Count: %d\n", fbc -> fbc_blocknum, fbc->fbc_free[fbc -> fbc_count-1], fbc->fbc_count);

		// Link old tail to new fbc node
		curr -> fbc_next = (struct fbcnode *)Newfbc -> fbc_blocknum;
		printf("Old tail next: %d\n", (int) curr -> fbc_next);

		// Need to write these changes to disk
		// Tell disk about the new fbc node
		seek(diskfd, Newfbc -> fbc_blocknum);
		if(NULL == write(diskfd, Newfbc, sizeof(struct fbcnode))){
			printf("Error writing new FBC node to disk \n"); // Debug line
			signal(supertab -> sb_freelock);
			return SYSERR;
		}
		// Update old tail
		seek(diskfd, curr -> fbc_blocknum);
		if(NULL == write(diskfd, curr, sizeof(struct fbcnode))){
			printf("Error updating old FBC tail \n"); // Debug line
			signal(supertab -> sb_freelock);
			return SYSERR;
		}

	}
	// If there's space in tail FBC node, just insert block in the node
	else{
		printf("Placing block = %d, at tail FBC node\n", block);
		// Back to disk!
		curr -> fbc_free[curr->fbc_count] = block; // Insert given block into FBC nodes last position
		curr -> fbc_count++; 

		// Write updated info to disk
		seek(diskfd, curr->fbc_blocknum);
    		if(NULL == write(diskfd, curr, sizeof(struct fbcnode))){
			signal(supertab -> sb_freelock);
			return SYSERR; // Does SYSERR use signal to release lock?
		}
	}	
	signal(psuper -> sb_freelock);
	return OK;
}
