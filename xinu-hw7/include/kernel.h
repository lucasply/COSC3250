/**
 * @file kernel.h 
 *
 * The base include file for the Xinu kernel. Defines symbolic constants,
 * universal return constants, intialization constants, machine size
 * definitions, inline utility functions, and include types
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <stddef.h>
#include <stdarg.h>
#include <compiler.h>

/* Kernel function prototypes */
void nulluser(void);

/* Kernel function prototypes */
syscall kprintf(const char *fmt, ...);
syscall kputc(uchar);
syscall kungetc(uchar);
syscall kgetc(void);
syscall kcheckc(void);
syscall kvprintf(const char *fmt, va_list va);

syscall create(void *, ulong, uint, char *, ulong, ...);
syscall kill(pid_typ);
syscall ready(pid_typ, bool);
syscall resched(void);

extern void _start(void);

// Counter for unget buffer
extern unsigned int bufp;

#define PREEMPT 	FALSE
#define SEED		1431    /* seed for random number generator */


/* Assertions */
#define ASSERT(cond)              if ( !(cond) ) return SYSERR
#define ASSERTFUNC(func_call)     ASSERT((func_call) == OK)

#endif                          /* _KERNEL_H_ */
