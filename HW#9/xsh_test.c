/**
 * @file     xsh_test.c
 * @provides xsh_test
 *
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>
void printFreeList(void);

/**
 * Shell command (test) is testing hook.
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */
command xsh_test(int nargs, char *args[])
{
    //TODO: Test your O/S.
    printf("This is where you should put some testing code.\n");

    uchar c;
	int block, i;

    printf("===TEST BEGIN===\n");
    printf("0): Test that file delete starts with block 63\n");
    printf("1): Test allocating and imediatly freeing all memory one at a time\n");
    printf("2): Test alocating all memory\n");

    c = kgetc();
    switch(c) {
        case '0':
		        // Write a test for checking that file delete starts with the correct file
				printFreeList();
				block = sbGetBlock(supertab);
				printf("Getting Block %d\n", block);
				
				sbFreeBlock(supertab, block);
				printf("Returning Block %d\n", block);
				
				printFreeList();

                break;
        case '1':
                // Write the test case that we did in class
				block = 0;
				
				printFreeList();
				
				for(i = 0; i < DISKBLOCKTOTAL; i++) {
					block = sbGetBlock(supertab);
					
					printf("Block %d = %d\n", i, block);
					sbFreeBlock(supertab, block);
					printFreeList();
					
					if ((i < 5) || ((i > 54) && (i < 63))) {
						int c = getc(CONSOLE);
					}

                break;
        case '2':
                // Write a test case for allocating all memory in the system
				block = 0;
				
				blockList[DISKBLOCKTOTAL];
				
				printFreeList();
				
				printf("Grabbing everything in the free list\n");
				
				for(i = 0; i < DISKBLOCKTOTAL; i++) {
					block = sbGetBlock(supertab);
					
					blockList[i] = block;
					
					if ((i == 6) || (i == 55) || (i == 64))) {
						printFreeList();
					}
				}
				
				printf("Grabed everything from the free list\n");
				printFreeList();
				
				printf("Returning everything to the free list\n");
				
				for(i = 0; i < DISKBLOCKTOTAL; i++) {
					sbFreeBlock(supertab, blockList[i]);
				}
				
				printf("Everything returned to the free list\n");
				printFreeList();
				
                break;
        default:
                break;
    }

    printf("\n===TEST END===\n");

    return OK;
}