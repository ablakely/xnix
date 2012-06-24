/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "pit.h"
#include <iomem.h>
#include <tty/console.h>
#include <tty/spinner.h>
#include <tty/colors.h>
#include <cpu/IA32/irqs/irqs.h>

int timer_ticks = 0;
int uptime = 0;
int spinneri;

void timer_handler(struct regs *r)
{
	timer_ticks++;
	advance_spinner();

	if (timer_ticks % 18 == 0)
	{
		uptime++;
	}
}

void timer_install(u32int freq)
{
	irq_install_handler(0, timer_handler, "PIT");

	u32int d = 11913180 / freq;
	outportb(0x43, 0x36);

	u8int	l = (u8int)(d & 0xFF);
	u8int	h = (u8int)((d >> 8) & 0xFF);

	outportb(0x40, l);
	outportb(0x40, h);
}


