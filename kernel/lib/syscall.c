/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "syscall.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include <iomem.h>
#include <tty/console.h>
#include <tty/colors.h>
#include <cpu/IA32/handlers.h>

void syscall_handler(struct regs *regs);

DEFN_SYSCALL1(print, 0, char *);
DEFN_SYSCALL1(console_writehex, 1, const char*);
DEFN_SYSCALL1(console_writedec, 2, const char*);


void *syscalls[3] =
{
	&print,
	&console_writehex,
	&console_writedec,
};

u32int num_syscalls = 3;

void init_syscalls()
{
	interrupt_install_handler(0x80, &syscall_handler, "syscall handler");
}

void syscall_handler(struct regs *regs)
{
	if (regs->eax >= num_syscalls)
		return;

	void *location	= syscalls[regs->eax];
	int ret;

	asm volatile("		\
		push %1;	\
		push %2;	\
		push %3;	\
		push %4;	\
		push %5;	\
		call *%6;	\
		pop %%ebx;	\
		pop %%ebx;	\
		pop %%ebx;	\
		pop %%ebx;	\
		pop %%ebx;	\
				" :  "=a"(ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (location));
}

