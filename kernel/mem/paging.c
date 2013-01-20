/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "paging.h"
#include <stdint.h>
#include <iomem.h>
#include <stdio.h>
#include <panic.h>
#include <tty/console.h>
#include <cpu/IA32/handlers.h>

extern u32int end;

u32int *create_page_table(int page_entry)
{
	u32int *page_table = page_directory + 0x1000;

	u32int address = 0;
	u32int i;

	for (i = 0; i < 1024; i++)
	{
		page_table[i]			= address | 3;
		address				= address + 4096;
	}

	// Copy the table into the directory
	page_directory[page_entry]		= (u32int*)page_table;
	page_directory[page_entry]	       |= 3;

	return page_table;
}

void init_paging()
{
	page_aligned_end = (end & 0xFFFFF000) + 0x1000;
	page_directory   = (u32int*)page_aligned_end;

	int i = 0;
	for (i = 0; i < 1024; i++)
	{
		page_directory[i]	= 0 | 2;
	}

	// Create the first page table
	u32int *first_page_table = create_page_table(0);

	// copy the page_directory pointer into the cr3 register
	asm volatile("mov %0, %%cr3" :: "b"(page_directory));

	u32int cr0;
	asm volatile("mov %%cr0, %0" : "=b"(cr0));	// read the register
	cr0 |= 0x80000000;				// flip the bit
	asm volatile("mov %0, %%cr0" :: "b"(cr0));	// write the new value and enable paging
}
