/**
 * @file freemem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup memory_mgmt
 *
 * Frees a block of heap-allocated memory.
 *
 * @param memptr
 *      Pointer to memory block allocated with getmem().
 *
 * @param nbytes
 *      Length of memory block, in bytes.  (Same value passed to getmem().)
 *
 * @return
 *      ::OK on success; ::SYSERR on failure.  This function can only fail
 *      because of memory corruption or specifying an invalid memory block.
 */
syscall freemem(void *memptr, ulong nbytes)
{
    register struct memblock *block, *next, *prev;
    struct memhead *head = NULL;
    ulong top;
	
    /* make sure block is in heap */
    if ((0 == nbytes)
        || ((ulong)memptr < (ulong)PROCHEAPADDR))
    {
        return SYSERR;
    }

    head = (struct memhead *)PROCHEAPADDR;
    block = (struct memblock *)memptr;
    nbytes = (ulong)roundmb(nbytes);

    /* TODO:
     *      - Find where the memory block should
     *        go back onto the freelist (based on address)
     *      - Find top of previous memblock
     *      - Make sure block is not overlapping on prev or next blocks
     *      - Coalesce with previous block if adjacent
     *      - Coalesce with next block if adjacent
     */

    // Heads up I get confused between * and & so I may have used the wrong ones
    // Start at beginning of free list
    memblk *currBlock = head;
    memblk *prevBlock = NULL;

    // Go through list unitl we find the block right before memptr
    while (block > currBlock) {
        // If we reach the end of the list something went wrong
        if (currBlock->next == NULL) {
                return SYSERR;
        }

        prevBlock = currBlock;
        currBlock = currBlock->next;
    }

    if ((prevBlock < block) && ((prevBlock + prevBlock->length) == block)) { // Possible fence post error
        // Merge with previous block
    }
    else {
        // Inster memptr into free list
        prevBlock->next = block;
        block->next = currBlock;
    }
	
    // Go through list until we find the block right after memptr
    while (block > currBlock) {
        // If we reach the end then stop trying
        if (currBlock->next == NULL) {
                return OK;
        }

        currBlock = currBlock->next;
    }

    if (currBlock == (block + block->length)) { // Possible fence post error
        // Merge with next block
    }

    return OK;
}