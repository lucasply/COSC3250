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

[lwagner39@argolis system]$ cat testcases.c
/**
 * @file testcases.c
 * @provides testcases
 *
 * TA-BOT:MAILTO
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

extern void main(int, char *);

int test_usernone(void) {
        kprintf("This is a test of ...");
        user_none();
        kprintf("user_none() syscall\r\n");

        return 0;
}

int test_userputc(void) {
        user_putc(0, 'H');
        user_putc(0, 'e');
        user_putc(0, 'l');
        user_putc(0, 'l');
        user_putc(0, 'o');
        user_putc(0, ' ');
        user_putc(0, 'W');
        user_putc(0, 'o');
        user_putc(0, 'r');
        user_putc(0, 'l');
        user_putc(0, 'd');
        user_putc(0, '!');
        user_putc(0, '\r');
        user_putc(0, '\n');

        return 0;
}

int test_usergetc(void) {
        char c;
        kprintf("Echo characters until 'X': ");
        while ((c = user_getc(0)) != 'X')
        {
                user_putc(0, c);
        }
        kprintf("\r\n");

        return 0;
}

int testmain(int argc, char **argv)
{
    int i = 0;
    kprintf("Hello XINU World!\r\n");

    for (i = 0; i < 10; i++)
    {
        kprintf("This is process %d\r\n", currpid);

        user_yield();
    }
    return 0;
}

void testbigargs(int a, int b, int c, int d, int e, int f, int g, int h)
{
    kprintf("Testing bigargs...\r\n");
    kprintf("a = 0x%08X\r\n", a);
    kprintf("b = 0x%08X\r\n", b);
    kprintf("c = 0x%08X\r\n", c);
    kprintf("d = 0x%08X\r\n", d);
    kprintf("e = 0x%08X\r\n", e);
    kprintf("f = 0x%08X\r\n", f);
    kprintf("g = 0x%08X\r\n", g);
    kprintf("h = 0x%08X\r\n", h);
}

void printpcb(int pid)
{
    pcb *ppcb = NULL;

    /* Using the process ID, access it in the PCB table. */
    ppcb = &proctab[pid];

    /* Printing PCB */
    kprintf("Process name                 : %s \r\n", ppcb->name);

    switch (ppcb->state)
    {
    case PRFREE:
        kprintf("State of the process     : FREE \r\n");
        break;
    case PRCURR:
        kprintf("State of the process     : CURRENT \r\n");
        break;
    case PRSUSP:
        kprintf("State of the process     : SUSPENDED \r\n");
        break;
    case PRREADY:
        kprintf("State of the process     : READY \r\n");
        break;
    default:
        kprintf("ERROR: Process state not correctly set!\r\n");
        break;
    }

    /* Print PCB contents and registers */
    kprintf("Base of run time stack    : 0x%08X \r\n", ppcb->stkbase);
    kprintf("Stack length of process   : %8u \r\n", ppcb->stklen);
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;

    kprintf("0) Test user_none syscall\r\n");
    kprintf("1) Test user_getc syscall\r\n");
    kprintf("2) Test user_putc syscall\r\n");
    kprintf("3) Create three processes that test user_yield syscall\r\n");

    kprintf("===TEST BEGIN===\r\n");

    // TODO: Test your operating system!

    c = kgetc();
    switch (c)
    {
    case '0':
        ready(create((void *)test_usernone, INITSTK, "test_usernone", 0),
              RESCHED_YES);
                break;

    case '1':
        ready(create((void *)test_usergetc, INITSTK, "test_usergetc", 0),
              RESCHED_YES);
        break;

    case '2':
        ready(create((void *)test_userputc, INITSTK, "test_userputc", 0),
              RESCHED_YES);
                break;

    case '3':
        // Create three copies of a process, and let them play.
        ready(create((void *)testmain, INITSTK, "MAIN1", 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, "MAIN2", 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, "MAIN3", 2, 0, NULL),
              RESCHED_YES);
        while (numproc > 1)
            resched();
        break;

    default:
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
