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

void timer_install()
{
	irq_install_handler(0, timer_handler, "PIT");
}


