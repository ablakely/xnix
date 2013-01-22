/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "panic.h"
#include <iomem.h>
#include <stdio.h>
#include <tty/console.h>
#include <tty/spinner.h>
#include <tty/colors.h>

void panic(char *err, struct regs *regs)
{
	printc("\nPANIC: ", BLACK, RED);
	printf("%s\n", err);

	if (regs)
	{
		u32int cr0, cr3;
		asm volatile("mov %%cr0, %0" : "=r"(cr0));
		asm volatile("mov %%cr3, %0" : "=r"(cr3));

		printf("\n\nRegister states at interrupt:\n");
		printf("  eax: ................. %x\n  ebx: ................. %x\n  ecx: ................. %x\n", regs->eax, regs->ebx, regs->ecx);
		printf("  edx: ................. %x\n  esp: ................. %x\n  ebp: ................. %x\n", regs->edx, regs->esp, regs->ebp);
		printf("  eip: ................. %x\n  cr0: ................. %x\n  cr3: ................. %x\n", regs->eip, cr0, cr3);
		printf("  Error Code: .......... %x\n", regs->err_code);
		printf("  EFLAGS: .............. %x\n", regs->eflags);
		printf("  User ESP: ............ %x\n", regs->useresp);
	}

	print("\n");

	for (;;)
	{
		// do nothing
	}
}

void panic_assert(const char *file, u32int line, const char *desc)
{
	asm volatile("cli");

	printc("ASSERTION FAILED:", BLACK, RED);
	printf("%s at %s : %d\n", desc, file, line);

	for(;;);
}
