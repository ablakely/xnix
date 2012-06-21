/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "malloc.h"
#include <stdint.h>

extern u32int end;
u32int placement_address = (u32int)&end;

u32int xmalloc_int(u32int sz, int align, u32int *phys)
{
	if (align == 1 && (placement_address & 0xFFFFF000))
	{
		placement_address &= 0xFFFFF00;
		placement_address += 0x1000;
	}

	if (phys)
	{
		*phys = placement_address;
	}

	u32int tmp = placement_address;
	placement_address += sz;
	return tmp;
}

u32int xmalloc(u32int sz)
{
	return xmalloc_int(sz, 1, 0);
}

u32int xmalloc_(u32int sz, u32int *phys)
{
	return xmalloc_int(sz, 0, phys);
}

u32int xmalloc_p(u32int sz, u32int *phys)
{
	return xmalloc_int(sz, 1, phys);
}

u32int malloc(u32int sz)
{
	return xmalloc_int(sz, 0, 0);
}
