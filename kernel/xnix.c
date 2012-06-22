/*
 * xnix: small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include <iomem.h>
#include <stdio.h>
#include <panic.h>
#include <tty/console.h>
#include <tty/spinner.h>
#include <tty/colors.h>
#include <cpu/IA32/IA32.h>
#include <cpu/IA32/gdt/gdt.h>
#include <cpu/IA32/idt/idt.h>
#include <cpu/IA32/isrs/isrs.h>
#include <cpu/IA32/irqs/irqs.h>
#include <io/kb/kb.h>
#include <io/pit/pit.h>
#include <mem/paging.h>

#define halt() for(;;);

int xnix_main()
{
	int kernel_ticks = 0;

	init_console();
	print("xnix 0.0.1 (by Aaron Blakely)\n\n");

	gdt_install();		// install the gdt
	idt_install();		// install the idt
	isrs_install();		// install the ISRs
	irq_install();		// install the IRQs
	__asm__ volatile("sti"); // safe to allows IRQs now.

	timer_install();
	keyboard_install();	// install the keboard

	disable_spinner();
	cpu_wait();
	init_paging();
	cpu_wait();

	print("\n\n");

	u32int *ptr = (u32int *)0xA0000000;
	u32int dpf = *ptr;

	// loop forever to keep the system alive
	for (;;)
	{
		kernel_ticks++;

	}
}
