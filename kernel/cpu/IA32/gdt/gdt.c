/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "gdt.h"
#include <iomem.h>
#include <stdint.h>
#include <stdio.h>
#include <tty/console.h>
#include <tty/colors.h>

extern void gdt_flush(u32int);
struct gdt_entry gdt[3];
struct gdt_ptr   gp;

void gdt_set_gate(int num, u32int base, u32int limit, u8int access, u8int gran)
{
	gdt[num].base_low	= (base & 0xFFFF);
	gdt[num].base_middle	= (base >> 16) & 0xFF;
	gdt[num].base_high	= (base >> 24) & 0xFF;

	gdt[num].limit_low	= (limit & 0xFFFF);
	gdt[num].granularity	= ((limit >> 16) & 0x0F);

	gdt[num].granularity   |= gran & 0xF0;
	gdt[num].access		= access;
}

void gdt_install()
{
	printc("Installing ", BLACK, LIGHT_GREEN);
	printc("GDT\n", BLACK, LIGHT_RED);

	gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
	gp.base  = (u32int)&gdt;

	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	gdt_flush((u32int)&gp);
}
