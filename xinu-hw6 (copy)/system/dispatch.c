/**
 * COSC 3250 - Project 5
 * Handles traps
 * @author [Lucas Wagner, Jonah Harlan]
 * Instructor [Professor Brylow]
 * TA-BOT:MAILTO [l.wagner@marquette.edu, jonah.harlan@marquette.edu]
 */
/**
 * @file dispatch.c
 * @provides dispatch
 *
 */
/* Embedded XINU, Copyright (C) 2008, 2023.  All rights reserved. */


#include <xinu.h>

/**
 * @ingroup process
 * Dispatch the trap or exception handler, called via interrupt.S
 * @param cause  The value of the scause register 
 * @param stval  The value of the stval register  
 * @param frame  The stack pointer of the process that caused the interupt 
 * @param program_counter  The value of the sepc register 
 */

void dispatch(ulong cause, ulong val, ulong *frame, ulong *program_counter) {
    ulong swi_opcode;
    
    if((long)cause > 0) {
        cause = cause << 1;
        cause = cause >> 1;

       /**
	* TODO:
	* Check to ensure the trap is an environment call from U-Mode
	* Find the system call number that's triggered
	* Pass the system call number and any arguments into syscall_dispatch.
	* Make sure to set the return value in the appropriate spot.
	* Update the program counter appropriately with set_sepc*
	* If the trap is not an environment call from U-Mode call xtrap
	*/
		
	// Check for U-Mode
	if(cause == E_ENVCALL_FROM_UMODE){
		// Get syscall num (Stored in A7, see top of syscall_dispatch.c		       
		swi_opcode = frame[CTX_A7];
		// Pointer to args
		ulong *args = &frame[CTX_A0];
		// Pass syscall to syscall_dispatch as well as args, and save return value in A0
		frame[CTX_A0] = syscall_dispatch(swi_opcode, args);
		// Increment PC address to avoid ecall loop, want next intruction of PC after the ecall
		// set_sepc sets the value of (X) into sepc
		ulong X = (ulong)program_counter + 4;
		set_sepc(X);
	
	}
    } else{
		cause = cause << 1;
    		cause = cause >> 1;
    		uint irq_num;

    		volatile uint *int_sclaim = (volatile uint *)(PLIC_BASE + 0x201004);
    		irq_num = *int_sclaim;

    		if(cause == I_SUPERVISOR_EXTERNAL) {
        		interrupt_handler_t handler = interruptVector[irq_num];

        		*int_sclaim = irq_num;
        		if (handler)
        		{	
            			(*handler) ();
        		} else {
            			kprintf("ERROR: No handler registered for interrupt %u\r\n",
                    		irq_num);

            			while (1)
                		;
        		}
    		}
	}
}

