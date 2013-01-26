/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "handlers.h"
#include <stdint.h>
#include <stdio.h>
#include <iomem.h>
#include <panic.h>
#include "idt/idt.h"
#include "isrs/isrs.h"
#include "irqs/irqs.h"
#include "tty/console.h"
#include "tty/colors.h"

isr_t interrupt_handlers[256];

void init_handlers()
{
	memset(&interrupt_handlers, 0, sizeof(isr_t)*256);
}

void interrupt_install_handler(u8int n, isr_t handler, char *msg)
{
	printc("Installing handler: ", BLACK, LIGHT_GREEN);
	printf("%s\n", msg);

	interrupt_handlers[n] = handler;
}

void interrupt_remove_handler(u8int n)
{
	interrupt_handlers[n] = 0;
}

void isr_handler(registers_t *r)
{
	u8int int_no = r->int_no & 0xFF;		// for conversion of 8-bit to 32bit

	if (interrupt_handlers[int_no] != 0)
	{
		isr_t handler = interrupt_handlers[int_no];
		handler(r);
	}
	else
	{
		panic("Encountered unmaskable interrupt!", r);
	}
}

void irq_handler(registers_t *r)
{
	u8int int_no = r->int_no & 0xFF;                // for conversion of 8-bit to 32bit

	if (int_no >= 40)
	{
		outportb(0xA0, 0x20);
	}

	outportb(0x20, 0x20);
	if (interrupt_handlers[int_no] != 0)
	{
		isr_t handler = interrupt_handlers[int_no];
		handler(r);
	}
}
