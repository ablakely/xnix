/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "tss.h"
#include <iomem.h>
#include <stdint.h>
#include <cpu/IA32/gdt/gdt.h>
#include <cpu/IA32/descriptors.h>

tss_entry_t tss_entry;
extern void tss_flush();

void write_tss(s32int num, u16int ss0, u32int esp0)
{
	u32int base		= (u32int) &tss_entry;
	u32int limit		= base + sizeof(tss_entry);

	gdt_set_gate(num, base, limit, 0xE9, 0x00);

	memset(&tss_entry, 0, sizeof(tss_entry));

	tss_entry.ss0		= ss0;		// kernel stack segment
	tss_entry.esp0		= esp0;		// kernel stack pointer

	tss_entry.cs		= 0x0b;
	tss_entry.ss		= tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}

void set_kernel_stack(u32int stack)
{
	tss_entry.esp0		= stack;
}


