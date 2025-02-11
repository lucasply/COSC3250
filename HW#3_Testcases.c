/**
 * @file testcases.c
 * @provides testcases
 *
 * Modified by: Lucas Wagner
 *
 * and Jonah Harlan
 *
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <xinu.h>

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;

    kprintf("===TEST BEGIN===\r\n");

    c = kgetc();
    switch (c)
    {

        // TODO: Test your operating system!
    case 'a':
            kprintf("\r\n123\r\n");
    case 'b':
            kprintf("\r\n---32 character test---\r\n");

            kprintf("\r\nabcdefghijklmnopqrstuvwxyz123456732 \r\n");

    case 'c':
            if(kgetc()){
            kprintf("\r\n---kgetc() is working---\r\n");
            }

    case 'd':
            kprintf("\r\nMulti-byte character test\r\n");

            kprintf("\r\nShould be Euro symbol: €\r\n");

    case 'e':
            kprintf("\r\nkputc() test: ");
            kputc(c);

    default:
        kprintf("\r\nHello Xinu World!\r\n");
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
