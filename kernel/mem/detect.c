/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

__asm__(".code16gcc\n"); // running in real mode requires

#include "detect.h"
#include <inttypes.h>

int __attribute__((noinline)) __attribute__((regparam(3))) detectMemory(SMAP_entry_t *buffer, int maxentries)
{
	uint32 countID = 0;
	int entries = 0, signature, bytes;
	do
	{
		__asm__ volatile ("int $0x15"
				: "=a" (signature), "=c" (bytes), "=b" (countID)
				: "a" (0xE820), "b" (countID), "c" (24), "d" (0x534D4150), "D" (buffer));

		if (signature != 0x534D4150)
			return -1;

		if (bytes > 20 && (buffer->ACPI & 0x0001) == 0)
		{
			// ignore this entry
		}
		else {
			buffer++;
			entries++;
		}
	}
	while (countID != 0 && entries < maxentries);
	return entries;
}

