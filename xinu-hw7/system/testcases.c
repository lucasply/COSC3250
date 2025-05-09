/**
 * @file testcases.c
 * @provides testcases
 *
 *
 * Modified by:	
 *
 * TA-BOT:MAILTO 
 *
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <xinu.h>

/* This fake page table will allow you to test your printPageTable function
 * without having paging completely working.
 */
pgtbl createFakeTable(void){
	pgtbl root = pgalloc();
	pgtbl lvl1 = pgalloc();
	pgtbl lvl0 = pgalloc();

	volatile ulong *pte = &(root[5]);
	*pte = PA2PTE(lvl1) | PTE_V;

	ulong *lvl1pte = &(lvl1[145]);
	*lvl1pte = PA2PTE(lvl0) | PTE_V;

	ulong *lvl0pte = &(lvl0[343]);
	*lvl0pte = PA2PTE(0x1000) | PTE_W | PTE_R | PTE_V;

	ulong *lvl0pte1 = &(lvl0[120]);
	*lvl0pte1 = PA2PTE(0x4000) | PTE_X | PTE_R | PTE_V;

	ulong *lvl0pte2 = &(lvl0[45]);
	*lvl0pte2 = PA2PTE(0x8000) | PTE_X | PTE_R | PTE_V;

	return root;
}

void printPageTable(pgtbl pagetable)
{
	/*
	* TODO: Write a function that prints out the page table.
	* Your function should print out all *valid* page table entries in
	* the page table.  If any of the entries are a link (if the
	* Read/Write/Execute bits aren't set), recursively print that page
	* table.  If it is a leaf, print the page table entry and the
	* physical address is maps to. 
	*/
	//kprintf("Printing page table... \n\r");
	static int level = 2, i=0;
	if (!pagetable){
		kprintf("Invalid pagetable\n\r");
		return;
	}
    	for (i = 0; i < PTE_MAX; i++)
    	{
		pte entry  = pagetable[i];

        	if ((entry & PTE_V) && !(entry & ((entry&PTE_R)|(entry&PTE_W)|(entry&PTE_X))))
        	{
			kprintf("Level: %d  PTE: 0x%08lx \n\r ", level, entry);
            		ulong pa = PTE2PA(entry);
			level--;
			printPageTable((pgtbl)pa);
		}

		else if(entry&PTE_V){
				ulong pa = PTE2PA(entry);
            			kprintf("VPN[%d] -> PA: 0x%08lx, Flags: ", i, pa);
            			if (entry & PTE_R) kprintf("R");
            			if (entry & PTE_W) kprintf("W");
     				if (entry & PTE_X) kprintf("X");
       				if (entry & PTE_U) kprintf("U");
				if (entry & PTE_G) kprintf("G");
				if (entry & PTE_A) kprintf("A");
				if (entry & PTE_D) kprintf("D");
				kprintf("\n\r");
				level = 2;
		}
	}
}
			    		

void work(int a, int b){
	// kprintf("Work starting \r\n");
	int x = a + b*100;
	int i;
	for(i=0; i<x; i++){
		x += i;
	}
	// kprintf("User process work has finished \n\r");
}
int *OutOfBounds(int *ptr){
	*ptr = *ptr + 100000000;
	return ptr;
}
void PermissionCheck(void){
	pcb *ppcb = &proctab[currpid];
	kprintf("Reading kernel variable, Process State: %d\r\n", ppcb->state); // Should print the pid
	ppcb->state = 99; // Should trigger a store page fault
	kprintf("This should not print\r\n");
}
/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
	uchar c;

	kprintf("===TEST BEGIN===\r\n");
	kprintf("(0): Create a process and print it's page table\n\r");
	kprintf("(1): Test user process ability to access memory, should result in an error\n\r");
        kprintf("(2): Test whether a user process can access kernal variables or not \n\r");	

	// TODO: Test your operating system!

	c = kgetc();
	switch (c)
	{
		case '0':
			// TODO: Write a testcase that creates a user process
			// and prints out it's page table
			// Making UserProc
			uint pid = create((void *)work, INITSTK, 100, "MAIN1", 2, 1, 2);
		        //Print the page table
			//pgtbl pagetable = createFakeTable();
//			kprintf("Process: %d \n\r", pid);
			pgtbl pagetable = proctab[pid].pagetable; 
			printPageTable(pagetable);
//			kprintf("Going into ready Q \n\r");
			ready(pid, RESCHED_YES);
			break;
		case '1':
			// TODO: Write a testcase that demonstrates a user
			// process cannot access certain areas of memory

			uint pid1 = create((void *)OutOfBounds, INITSTK, 100, "MAIN1", 1, 0);
			kprintf("Starting test\n\r");
			ready(pid1, RESCHED_YES);
			break;
		case '2':
			// TODO: Write a testcase that demonstrates a user
			// process can read kernel variables but cannot write
			// to them
							
			uint pid2 = create((void *)PermissionCheck, INITSTK, 100, "Main1", 1, NULL);
			kprintf("Starting test 2\n\r");
			ready(pid2, RESCHED_YES);		
			
			break;
		case '3':
			// TODO: Extra credit! Add handling in xtrap to detect
			// and print out a Null Pointer Exception.  Write a
			// testcase that demonstrates your OS can detect a
			// Null Pointer Exception.
			break;
		default:
			break;
	}

	kprintf("\r\n===TEST END===\r\n");
	return;
}
