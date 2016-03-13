/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "callhandler.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "panic.h"
#include <iomem.h>
#include <tty/console.h>
#include <tty/colors.h>
#include <cpu/IA32/handlers.h>
#include <proc/task.h>

void xexit(int retv)
{
	task_exit(retv);
}

void syscall_handler(registers_t *regs);

DEFN_SYSCALL1(put, 0, char*);
DEFN_SYSCALL0(getpid, 1);
DEFN_SYSCALL1(console_writedec, 2, u32int);
DEFN_SYSCALL1(console_writehex, 3, u32int);
DEFN_SYSCALL0(fork, 4);
DEFN_SYSCALL1(xexit, 5, int);

void *syscalls[6] =
{
	&put,
	&getpid,
	&console_writedec,
	&console_writehex,
	&fork,
	&xexit,
};

u32int num_syscalls = 6;

void syscall_handler(registers_t *regs)
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
	regs->eax = ret;
}

void init_syscalls()
{
        interrupt_install_handler(0x7f, syscall_handler, "syscall handler");
}
