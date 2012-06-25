/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "handlers.h"
#include <stdint.h>
#include <stdio.h>
#include <iomem.h>
#include "idt/idt.h"
#include "isrs/isrs.h"
#include "irqs/irqs.h"
#include "tty/console.h"
#include "tty/colors.h"

void (*interrupt_handlers[256])(struct regs *);

void init_handlers()
{
	memset(&interrupt_handlers, 0, sizeof(void*(struct regs *r))*256);
}

void interrupt_install_handler(u8int n, void (*handler)(struct regs *), char *msg)
{
	printc("Installing handler: ", BLACK, LIGHT_GREEN);
	printf("%s\n", msg);

	interrupt_handlers[n] = handler;
}

void interrupt_remove_handler(u8int n)
{
	interrupt_handlers[n] = 0;
}

void isr_handler(struct regs *r)
{
	void (*handler)(struct regs *r);
	printf("isr%d called.", r->int_no);
	for(;;);

	if (r->int_no < 32)
	{
		if (interrupt_handlers[r->int_no] != 0)
		{
			handler = interrupt_handlers[r->int_no];
			handler(r);
		}
		else {
			printf("PANIC: Encountered fatal error: [0x%d] %s\n", r->int_no, exception_messages[r->int_no]);
			for(;;);
		}
	}
}

void irq_handler(struct regs *r)
{
	void (*handler)(struct regs *r);

	if (r->int_no >= 40)
	{
		outportb(0xA0, 0x20);
	}

	outportb(0x20, 0x20);
	if (interrupt_handlers[r->int_no] != 0)
	{
		handler = interrupt_handlers[r->int_no];
		handler(r);
	}
}
