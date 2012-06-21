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
