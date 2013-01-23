/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "idt.h"
#include <iomem.h>
#include <stdio.h>
#include <tty/console.h>
#include <tty/colors.h>
#include <cpu/IA32/isrs/isrs.h>

struct idt_entry idt[256];
struct idt_ptr   idtp;

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
	idt[num].base_lo	= (base & 0xFFFF);
	idt[num].base_hi	= (base >> 16) & 0xFFFF;

	idt[num].sel		= sel;
	idt[num].always0	= 0;
	idt[num].flags		= flags | 0x60;
}

void idt_install()
{
	printc("Installing ", BLACK, LIGHT_GREEN);
	printc("IDT\n", BLACK, LIGHT_RED);

	idtp.limit	= (sizeof (struct idt_entry) * 256) - 1;
	idtp.base	= (int)&idt;

	memset(&idt, 0, sizeof(struct idt_entry) * 256);
	isrs_install();
	idt_flush();
}
