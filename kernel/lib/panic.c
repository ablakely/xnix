/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "panic.h"
#include <stdio.h>
#include <tty/console.h>
#include <tty/spinner.h>
#include <tty/colors.h>

void panic(char *err)
{
	printc("\n\nPANIC: ", BLACK, RED);
	printf("%s\n", err);
	disable_spinner();

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
