/*
 * xnix: a tiny play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "irqs.h"
#include <iomem.h>
#include <stdio.h>
#include <tty/console.h>
#include <tty/colors.h>
#include <cpu/IA32/idt/idt.h>
#include <cpu/IA32/isrs/isrs.h>

void irq_install()
{
	printc("Installing ", BLACK, LIGHT_GREEN);
	printc("IRQs\n", BLACK, LIGHT_RED);

	idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
	idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
	idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
	idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
	idt_set_gate(36, (unsigned)irq4, 0x08, 0x8E);
	idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
	idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
	idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);
	idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
	idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
	idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
	idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
	idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
	idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
	idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
	idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
	idt_set_gate(128, (unsigned)_isr128, 0x08, 0x8E);	// this is used for syscalls
}

