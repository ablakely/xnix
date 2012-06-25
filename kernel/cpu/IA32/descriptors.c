/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 * This is an abstraction layer built ontop of the code found in
 * idt/ and gdt/. -- See those.
 *
 */

#include <stdint.h>
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "isrs/isrs.h"
#include "irqs/irqs.h"
#include "handlers.h"

char *exception_messages[] = {
        "Division by Zero",
        "Debug",
        "Non-Maskable Interrupt",
        "Breakpoint",
        "Into Detected Overflow",
        "Out of Bounds",
        "Invalid Opcode",
        "No Coprocessor",
        "Double Fault",
        "Coprocessor Segment Overrun",
        "Bad TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection Fault",
        "Page Fault",
        "Unkown Interrupt",
        "Coprocessor Fault",
        "Alignment Check",
        "Machine Check",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved"
};

void init_descriptors()
{
	gdt_install();
	idt_install();
	init_handlers();
	irq_install();
}
