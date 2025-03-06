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
        //      kprintf("Dispatch(): extracted syscall num: %u \n\r", swi_opcode);
                // Pointer to args
                ulong *args = &frame[CTX_A0];
                // Pass syscall to syscall_dispatch as well as args, and save return value in A0
                frame[CTX_A0] = syscall_dispatch(swi_opcode, args);
        //      kprintf("Dispatch(): syscall_dispatch return: %1x\n\r", frame[CTX_A0]);
                // Increment PC address to avoid ecall loop, want next intruction of PC after the ecall
        //      kprintf("Before set_sepc: program_counter addr: 0x%08X, value: 0x%08X\n\r",
        //                                      program_counter, *program_counter);
                // set_sepc sets the value of (X) into sepc
                ulong X = (ulong)program_counter + 4;
                set_sepc(X);
        //      kprintf("After set_sepc: program_counter addr: 0x%08X, value: 0x%08X\n\r",
        //                                              &X, X);

        }
        else{
        //      kprintf("Dispatch(): Not in user mode\n\r");
                xtrap(frame, cause, val, program_counter);
        }


    }
}
