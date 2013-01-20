/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "malloc.h"
#include <stdint.h>
#include <stdio.h>

extern u32int end;
u32int placement_address = (u32int)&end;

u32int xmalloc_int(u32int sz, int align, u32int *phys)
{
//	printf("debug: pa=0x%d\n", placement_address);
	if (align == 1 && (placement_address & 0xFFFFF000))
	{
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}

	if (phys)
		*phys = placement_address;

	u32int tmp = placement_address;
	placement_address += sz;
	return tmp;
}

u32int xmalloc_a(u32int sz)
{
	return xmalloc_int(sz, 1, 0);
}

u32int xmalloc_p(u32int sz, u32int *phys)
{
	return xmalloc_int(sz, 0, phys);
}

u32int xmalloc_ap(u32int sz, u32int *phys)
{
	return xmalloc_int(sz, 1, phys);
}

u32int xmalloc(u32int sz)
{
	return xmalloc_int(sz, 0, 0);
}
