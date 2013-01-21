/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "paging.h"
#include "heap.h"
#include "malloc.h"
#include <stdint.h>
#include <iomem.h>
#include <stdio.h>
#include <panic.h>
#include <tty/console.h>
#include <tty/colors.h>
#include <cpu/IA32/handlers.h>

page_directory_t *kernel_directory = 0;
page_directory_t *current_directory = 0;

u32int *frames;
u32int nframes;

extern heap_t *xnix_heap;
extern u32int placement_address;

#define INDEX_FROM_BIT(a)  (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void set_frame(u32int frame_addr)
{
	u32int frame	= frame_addr/0x1000;
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
				if (!(frames[i]&toTest))
				{
					return i*4*8+j;
				}
			}
		}
	}
}

void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
	if (page->frame != 0)
	{
		return;		// frame already allocated
	}
	else {
		u32int idx = first_frame();
		if (idx == (u32int)-1)
		{
			panic("No free frames left -- Out of memory.");
		}
		set_frame(idx*0x1000);
		page->present = 1;
		page->rw      = (is_writeable)?1:0;
		page->user    = (is_kernel)?0:1;
		page->frame   = idx;
	}
}

void free_frame(page_t *page)
{
	u32int frame;
	if (!(frame = page->frame))
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
	u32int mem_end_page		= 0x1000000;
	nframes				= mem_end_page / 0x1000;
	frames				= (u32int *)xmalloc(INDEX_FROM_BIT(nframes));
	memset(frames, 0, INDEX_FROM_BIT(nframes));

	kernel_directory = (page_directory_t*)xmalloc_a(sizeof(page_directory_t));
	memset(kernel_directory, 0, sizeof(page_directory_t));
	current_directory = kernel_directory;

	int i = 0;
	for (i = XNIX_HEAP_START; i < XNIX_HEAP_START + XNIX_HEAP_INITIAL_SIZE; i += 0x1000)
	{
		get_page(i, 1, kernel_directory);
	}

	i = 0;
	while (i < placement_address)
	{
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}

	for (i = XNIX_HEAP_START; i < XNIX_HEAP_START + XNIX_HEAP_INITIAL_SIZE; i += 0x1000)
	{
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
	}

	interrupt_install_handler(14, page_fault, "page fault handler");
	switch_page_directory(kernel_directory);

	// Initialise the kernel heap
	printc("Initilizing kernel heap...\n", BLACK, WHITE);
	xnix_heap = create_heap(XNIX_HEAP_START, XNIX_HEAP_START + XNIX_HEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
}

void switch_page_directory(page_directory_t *dir)
{
	current_directory = dir;
	asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
	u32int cr0;

	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(u32int address, int make, page_directory_t *dir)
{
	address		/= 0x1000;
	u32int table_idx = address / 1024;

	if (dir->tables[table_idx])
	{
		return &dir->tables[table_idx]->pages[address%1024];
	}
	else if (make) {
		u32int tmp;
		dir->tables[table_idx]	= (page_table_t*)xmalloc_ap(sizeof(page_table_t), &tmp);
		dir->tablesPhysical[table_idx] = tmp | 0x7;

		return &dir->tables[table_idx]->pages[address%1024];
	}
	else {
		return 0;
	}
}

void page_fault(struct regs *r)
{
	u32int faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

	int present	= !(r->err_code & 0x1);
	int rw		= r->err_code & 0x2;
	int us		= r->err_code & 0x4;
	int reserved	= r->err_code & 0x8;
	int id		= r->err_code & 0x10;

	printf("\n\n\n\nPage Fault!  ( ");
	if (present)	printf("present ");
	if (rw)		printf("read-only ");
	if (us)		printf("user-mode ");
	if (reserved)	printf("reserved ");

	printf(") at 0x");
	console_writehex(faulting_address);
	printf("\n");
	panic("PAGE FAULT!");
}
