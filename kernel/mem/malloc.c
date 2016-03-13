/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "malloc.h"
#include "paging.h"
#include "heap.h"
#include <stdint.h>
#include <stdio.h>

extern u32int end;
extern page_directory_t *kernel_directory;
extern heap_t *xnix_heap;

u32int placement_address = (u32int)&end;

u32int xmalloc_int(u32int sz, int align, u32int *phys)
{
	if (xnix_heap != 0)
	{
		void *addr = alloc(sz, (u8int)align, xnix_heap);
		if (phys != 0)
		{
			page_t *page	= get_page((u32int)addr, 0, kernel_directory);
			*phys		= page->frame * 0x1000 + ((u32int)addr & 0xFFF);
		}

		return (u32int)addr;
	}
	else
	{
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

void xfree(void *p)
{
	free(p, xnix_heap);
}
