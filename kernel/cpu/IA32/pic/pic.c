/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "pic.h"
#include <iomem.h>
#include <cpu/IA32/IA32.h>

void pic_sendeoi(int irq)
{
	switch(irq)
	{
		case PIC_EOI_MASTER:
		{
			outportb(PIC1_COMMAND, 0x20);
			break;
		}
		case PIC_EOI_SLAVE:
		{
			outportb(PIC2_COMMAND, 0x20)
			break;
		}
		case PIC_EOI_ALL:
		{
			pic_sendeoi(PIC_EOI_MASTER);
			pic_sendeoi(PIC_EOI_SLAVE);
			break;
		}
		default:
		{
			if (irq >= 8)
				outportb(PIC2_COMMAND, 0x20);
			outportb(PIC1_COMMAND, 0x20);
			break;
		}
	}
}

void pic_remap(int off1, int off2)
{
	unsigned char mask1 = inportb(PIC1_DATA);
	unsigned char mask2 = inportb(PIC2_DATA);

	outportb(PIC1_COMMAND, 0x11); cpu_wait();
	outportb(PIC2_COMMAND, 0x11); cpu_wait();
	outportb(PIC1_DATA,    off1); cpu_wait();
	outportb(PIC2_DATA,    off2); cpu_wait();
	outportb(PIC1_DATA,    0x04); cpu_wait();
	outportb(PIC2_DATA,    0x02); cpu_wait();
	outportb(PIC1_DATA,    0x01); cpu_wait();
	outportb(PIC2_DATA,    0x01); cpu_wait();

	outportb(PIC1_DATA, mask1);
	outportb(PIC2_DATA, mask2);
}


