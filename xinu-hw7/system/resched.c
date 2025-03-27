/**
 * @file resched.c
 * @provides resched
 *
 * COSC 3250 Assignment 6
 */
/* Embedded XINU, Copyright (C) 2008,2024.  All rights reserved. */

#include <xinu.h>

extern void ctxsw(void *, void *, ulong);
/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate 
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */
syscall resched(void)
{
    pcb *oldproc;               /* pointer to old process entry */
    pcb *newproc;               /* pointer to new process entry */

    oldproc = &proctab[currpid];

    /* place current process at end of ready queue */
    if (PRCURR == oldproc->state)
    {
        oldproc->state = PRREADY;
        enqueue(currpid, readylist);
    }

    /**
     * We recommend you use a helper function for the following:
     * TODO: Get the total number of tickets from all processes that are in
     * current and ready states.
     * Use random() function to pick a random ticket. 
     * Traverse through the process table to identify which proccess has the
     * random ticket value.  Remove process from queue.
     * Set currpid to the new process.
     */
    uint total_tickets = 0;
    uint i;
    for(i =0; i<NPROC; i++){
	    if(proctab[i].state == PRREADY){
		   total_tickets += proctab[i].tickets;
	    }
    }
    // Select winner
    uint winner = random(total_tickets);
    //kprintf("---Resched(): Winning ticket: %d ---\r\n", winner);
    // Find process with winning ticket
    uint counter = 0;
    for(i =0; i<NPROC; i++){
    	if(proctab[i].state == PRREADY){
		counter += proctab[i].tickets;
		if(winner < counter){
			currpid = i;
			break;
		}
	}
    }
		
    remove(currpid); // Take out of readylist	
    newproc = &proctab[currpid];
    newproc->state = PRCURR;    /* mark it currently running    */
    kprintf("Resched(): Old Process Pid: %d\t New Process PID: %d \r\n", oldproc-proctab, newproc-proctab);
#if PREEMPT
    preempt = QUANTUM;
#endif

    ctxsw(&oldproc->ctx, &newproc->ctx,
	  MAKE_SATP(currpid, newproc->pagetable));

    /* The OLD process returns here when resumed. */
    return OK;
}
