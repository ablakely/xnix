/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "kb.h"
#include "map.h"
#include "kbbuffer.h"
#include <iomem.h>
#include <cpu/IA32/irqs/irqs.h>
#include <tty/console.h>
#include <tty/colors.h>

#define LEFT_SHIFT  1
#define RIGHT_SHIFT 2
#define CAPS_LOCK   3

int state = 0;

void keyboard_handler(struct regs *r)
{
	unsigned char scancode;
	scancode = inportb(0x60);

	if (scancode & 0x80)
	{
		if ((char)kbmap[scancode] == LEFT_SHIFT || (char)kbmap[scancode] == RIGHT_SHIFT)
		{
			state--;
		}
		else if (kbmap[scancode] == CAPS_LOCK)
		{
			if (state == 0)
			{
				state++;
			} else {
				state--;
			}
		}
	}
	else {
		if ((char)kbmap[scancode] == LEFT_SHIFT || (char)kbmap[scancode] == RIGHT_SHIFT)
		{
			state++;
		}
		else {
			if (state == 1)
			{
				put_c(uckbmap[scancode], BLACK, WHITE);
			}
			else {
				put_c(kbmap[scancode], BLACK, WHITE);
			}
		}
	}
}

void keyboard_install()
{
	irq_install_handler(1, keyboard_handler, "keyboard");
}
