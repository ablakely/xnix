/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "kb.h"
#include "map.h"
#include "buffer.h"
#include <iomem.h>
#include <cpu/IA32/irq.h>
#include <tty/console.h>
#include <tty/color.h>

state = 0;

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
				put(uckbmap[scancode]);
			}
			else {
				put(kbmap[scancode]);
			}
		}
	}
}

void keyboard_install()
{
	irq_install_handler(1, keyboard_handler, "keyboard");
}
