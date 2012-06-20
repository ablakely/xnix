/*
 * xnix: small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include <iomem.h>
#include <stdio.h>
#include <tty/console.h>
#include <tty/colors.h>
#include <cpu/IA32/gdt/gdt.h>
#include <cpu/IA32/idt/idt.h>
#include <cpu/IA32/isrs/isrs.h>
#include <cpu/IA32/irqs/irqs.h>
#include <io/kb/kb.h>

#define halt() for(;;);

int xnix_main()
{
	init_console();
	print("xnix: 0.0.1 by Aaron Blakely.  Starting.\n\n");

	gdt_install();		// install the gdt
	idt_install();		// install the idt
	isrs_install();		// install the ISRs
	irq_install();		// install the IRQs
	keyboard_install();	// install the keboard
	putc("\n");

	// loop forever to keep the system alive
	for (;;);
}
