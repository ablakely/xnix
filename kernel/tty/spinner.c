/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "spinner.h"
#include <stdio.h>
#include <stdint.h>
#include "console.h"
#include "colors.h"

int spinner_stop = 0;

void advance_spinner()
{
	if (spinner_stop == 1)
		return;

	static int pos = 0;
	char ch[4]    = {'/', '-', '\\', '|'};
	printc_to_point(ch[pos], 66, 25 / 2 - 0.5, BLACK, WHITE);
	print_to_point("[Loading]", 63, 25 / 2 -0.5 + 2, BLACK, RED);
	pos = (pos+1) % 4;
}

void disable_spinner()
{
	spinner_stop = 1;
}
