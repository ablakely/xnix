/*
 * xnix: small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include <mem/detect.h>
#include <tty/console.h>
#include <stdio.h>

#define halt() for(;;);

int xnix_main(multiboot_info *mbt, unsigned int magic)
{
	// loop forever to keep the system alive
	for (;;);
}
