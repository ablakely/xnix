/*
 * xnix: a tiny play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "isrs.h"
#include <stdint.h>
#include <iomem.h>
#include <tty/console.h>
#include <tty/colors.h>
#include <cpu/IA32/handlers.h>
#include <cpu/IA32/gdt/gdt.h>
#include <cpu/IA32/idt/idt.h>

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


void isrs_install()
{
	printc("Installing ", BLACK, LIGHT_GREEN);
	printc("ISRs\n", BLACK, LIGHT_RED);

	irq_remap();

	idt_set_gate(0, (u32int)_isr0, 0x08, 0x8E);
	idt_set_gate(1, (u32int)_isr1, 0x08, 0x8E);
	idt_set_gate(2, (u32int)_isr2, 0x08, 0x8E);
	idt_set_gate(3, (u32int)_isr3, 0x08, 0x8E);
	idt_set_gate(4, (u32int)_isr4, 0x08, 0x8E);
	idt_set_gate(5, (u32int)_isr5, 0x08, 0x8E);
	idt_set_gate(6, (u32int)_isr6, 0x08, 0x8E);
	idt_set_gate(7, (u32int)_isr7, 0x08, 0x8E);
	idt_set_gate(8, (u32int)_isr8, 0x08, 0x8E);
	idt_set_gate(9, (u32int)_isr9, 0x08, 0x8E);
	idt_set_gate(10, (u32int)_isr10, 0x08, 0x8E);
	idt_set_gate(11, (u32int)_isr11, 0x08, 0x8E);
	idt_set_gate(12, (u32int)_isr12, 0x08, 0x8E);
	idt_set_gate(13, (u32int)_isr13, 0x08, 0x8E);
	idt_set_gate(14, (u32int)_isr14, 0x08, 0x8E);
	idt_set_gate(15, (u32int)_isr15, 0x08, 0x8E);
	idt_set_gate(16, (u32int)_isr16, 0x08, 0x8E);
	idt_set_gate(17, (u32int)_isr17, 0x08, 0x8E);
	idt_set_gate(18, (u32int)_isr18, 0x08, 0x8E);
	idt_set_gate(19, (u32int)_isr19, 0x08, 0x8E);
	idt_set_gate(20, (u32int)_isr20, 0x08, 0x8E);
	idt_set_gate(21, (u32int)_isr21, 0x08, 0x8E);
	idt_set_gate(22, (u32int)_isr22, 0x08, 0x8E);
	idt_set_gate(23, (u32int)_isr23, 0x08, 0x8E);
	idt_set_gate(24, (u32int)_isr24, 0x08, 0x8E);
	idt_set_gate(25, (u32int)_isr25, 0x08, 0x8E);
	idt_set_gate(26, (u32int)_isr26, 0x08, 0x8E);
	idt_set_gate(27, (u32int)_isr27, 0x08, 0x8E);
	idt_set_gate(28, (u32int)_isr28, 0x08, 0x8E);
	idt_set_gate(29, (u32int)_isr29, 0x08, 0x8E);
	idt_set_gate(30, (u32int)_isr30, 0x08, 0x8E);
	idt_set_gate(31, (u32int)_isr31, 0x08, 0x8E);
}


