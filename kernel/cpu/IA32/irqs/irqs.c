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

void *irq_routines[16] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
};


void irq_install_handler(int irq, void (*handler)(struct regs *r), char *handler_name)
{
	printc("Installing ", BLACK, LIGHT_GREEN);
	printc("IRQ Handler: ", BLACK, GREEN);
	printc(handler_name, BLACK, LIGHT_CYAN);
	printc("\n", BLACK, BLACK);

	irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq)
{
	irq_routines[irq] = 0;
}

void irq_remap(void)
{
	outportb(0x20, 0x11);
	outportb(0xA0, 0x11);
	outportb(0x21, 0x20);
	outportb(0xA1, 0x28);
	outportb(0x21, 0x04);
	outportb(0xA1, 0x02);
	outportb(0x21, 0x01);
	outportb(0xA1, 0x01);
	outportb(0x21, 0x0);
	outportb(0xA1, 0x0);
}

void irq_install()
{
	printc("Installing ", BLACK, LIGHT_GREEN);
	printc("IRQs\n", BLACK, LIGHT_RED);
	irq_remap();

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
}

void irq_handler(struct regs *r)
{
	void (*handler)(struct regs *r);
	printf("IRQ called (ID: %d) (rNum: %d)\n", r->int_no - 32, r->int_no);

	/*
	 * It seems as if the norm is to subtract 32 from r->int_no
	 * but for some reason I am getting the PIT with r->int_no as 10.
	 * so a quick hack I did was to subtract 10 instead of 32 and it seemed to work.
	 *
	 * Will inspect as to why this is happening later, I suspect it has to do
	 * with my IDT code.
	 * - Dark_Aaron
	 */

	handler = irq_routines[r->int_no - 10];
	if (handler)
	{
		handler(r);
	}

	if (r->int_no >= 40)
	{
		outportb(0xA0, 0x20);
	}

	outportb(0x20, 0x20);
}
