/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "paging.h"
#include "malloc.h"
#include <stdint.h>
#include <iomem.h>
#include <stdio.h>
#include <stdlib.h>
#include <panic.h>
#include <tty/console.h>
#include <cpu/IA32/irqs/irqs.h>

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

page_directory_t *kernel_directory = 0;
page_directory_t *current_directory = 0;

u32int *frames;
u32int nframes;

// defined in heap.c
extern u32int placement_address;

static void set_frame(u32int fframe_addr)
{
	u32int frame	= frame/0x1000;
	u32int idx	= INDEX_FROM_BIT(frame);
	u32int off	= OFFSET_FROM_BIT(frame);
	frames[idx]    |= (0x1 << off);
}

static void clear_frame(u32int frame_addr)
{
	u32int frame	= frame_addr/0x1000;
	u32int idx	= INDEX_FROM_BIT(frame);
	u32int off	= OFFSET_FROM_BIT(frame);
	frames[idx]    &= ~(0x1 << off);
}

static u32int test_frame(u32int frame_addr)
{
	u32int frame	= frame_addr/0x1000;
	u32int idx	= INDEX_FROM_BIT(frame);
	u32int off	= OFFSET_FROM_BIT(frame);
	return (frames[idx] & (0x1 << off));
}

static u32int first_frame()
{
	u32int i, j;
	for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
	{
		if (frames[i] != 0xFFFFFFFF)
		{
			for (j = 0; j < 32; j++)
			{
				u32int toTest = 0x1 << j;
				if (!(frames[i] & toTest))
				{
					return i*4*8*j;
				}
			}
		}
	}
}

void alloc_frame(page_t *page, bool is_kernel, bool is_writeable)
{
	if (page->frame != 0)
	{
		return;
	}
	else {
		u32int idx = first_frame();
		if (idx == (u32int)-1)
		{
			panic("No free frames!");
		}
		set_frame(idx*0x1000);
		page->present	= 1;
		page->rw	= (is_writeable)?true:false;
		page->user	= (is_kernel)?false:true;
		page->frame	= idx;
	}
}

void free_frame(page_t *page)
{
	u32int frame;
	if (!(frame=page->frame))
	{
		return;
	}
	else {
		clear_frame(frame);
		page->frame = 0x0;
	}
}

void init_paging()
{
	// the size of the ram, for now assume 16MB.  Will implement detect.h soon.
	u32int mem_end_page = 0x1000000;

	nframes = mem_end_page / 0x1000;
	frames  = (u32int*)malloc(INDEX_FROM_BIT(nframes));
	memset((u8int *)frames, (u8int)0, (u32int)INDEX_FROM_BIT(nframes));

	kernel_directory  = (page_directory_t*)xmalloc(sizeof(page_directory_t));
	current_directory = kernel_directory;

	int i = 0;
	while (i < placement_address)
	{
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}

	irq_install_handler(3, page_fault, "Paging: Fault Handler");
	switch_page_directory((page_directory_t *)kernel_directory);
}

void switch_page_directory(page_directory_t *dir)
{
	current_directory = dir;
	__asm__ volatile("mov %0, %%cr3" :: "r"(&(dir->tablesPhysical)));
	u32int cr0;
	__asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x10000000; // enable paging
	__asm__ volatile("mov %0, %%cr0" :: "r"(cr0));
}

page_t *get_page(u32int address, int make, page_directory_t *dir)
{
	address /= 0x1000;
	u32int table_idx = address / 1024;

	if (dir->tables[table_idx])
	{
		return &dir->tables[table_idx]->pages[address%1024];
	}
	else if (make)
	{
		u32int tmp;
		dir->tables[table_idx]		= (page_table_t*)xmalloc_p(sizeof(page_table_t), &tmp);
		dir->tablesPhysical[table_idx]  = tmp | 0x7;
		return &dir->tables[table_idx]->pages[address%1024];
	}
	else {
		return 0;
	}
}

void page_fault(struct regs *regist)
{
	u32int faulting_address;
	__asm__ volatile("mov %%cr2, %0" : "=r" (faulting_address));

	// detailed error report
	bool present	= !(regist->err_code & 0x1);	// page not present
	bool rw		= regist->err_code & 0x2;	// write operation?
	bool us		= regist->err_code & 0x4;	// processor was in user-mode?
	bool reserved	= regist->err_code & 0x8;	// overwritten CPU-reserved bits of page entry?
	bool id		= regist->err_code & 0x10;	// caused by an instruction fetch?

	char *panicmsg;
	sprintf(panicmsg, "Page fault: (%?%?%?):%x\n",
		present, "present ",
		rw     , "read-only ",
		us     , "user-mode ",
		reserved,"reserved",
		faulting_address);
	panic(panicmsg);
}
