/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "idt.h"
#include <iomem.h>
#include <tty/console.h>
#include <tty/colors.h>

struct idt_entry idt[256];
struct idt_ptr   idtp;

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
	idt[num].base_lo	= (base & 0xFFFF);
	idt[num].base_hi	= (base >> 16) & 0xFFFF;

	idt[num].sel		= sel;
	idt[num].always0	= 0;
	idt[num].flags		= flags;
}

void idt_install()
{
	printc("Installing ", BLACK, LIGHT_GREEN);
	printc("IDT\n", BLACK, LIGHT_CYAN);

	idtp.limit	= (sizeof (struct idt_entry) * 256) - 1;
	idtp.base	= (int)&idt;

	memset((u8int *)&idt, 0, sizeof(struct idt_entry) * 256);

	idt_load();
}