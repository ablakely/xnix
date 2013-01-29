/*
 * param_parser.c: Used for parsing boot paramaters passed by GRUB and other Multiboot bootloaders.
 *
 * Written by Aaron Blakely <aaron@ephasic.org>
 */

#include  "param_parser.h"

bootparams_t *parse(char *cmdline)
{
	bootparams_t *ret;

	ret->argc	= 0;
	int ptr		= 0;
	int ws		= 0;

	while (cmdline[ptr] != '\0')
	{
		if (cmdline[ptr] != ' ')
		{
			ret->argv[ret->argc][ws] = cmdline[ptr];
			ws++;
		}
		else
		{
			ret->argv[ret->argc][ws] = '\0';

			ret->argc++;
			ws = 0;
		}

		ptr++;
	}

	ret->argc++;

	return ret;
}
