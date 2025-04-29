/**
 * @file     xsh_test.c
 * @provides xsh_test
 *
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

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

    printf("===TEST BEGIN===\n");
    printf("(0): Test that file delete starts with block 63\n");
    printf("(1): Test allocating and imediatly freeing all memory one at a time\n");
    printf("(2): Test alocating all memory\n");

    c = kgetc();
    switch(c) {
        case '0':
		        // Write a test for checking that file delete starts with the correct file

                break;
        case '1':
                // Write the test case that we did in class

                break;
        case '2':
                // Write a test case for allocating all memory in the system

                break;
        default:
                break;
    }

    printf("\n===TEST END===\n");

    return OK;
}