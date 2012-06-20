/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include <iomem.h>
#include <stdint.h>

void outportb(u16int port, u8int val)
{
	// allows us to write to IO device ports - kinda useful
	__asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (val));
}

void outportw(u16int port, u16int val)
{
	__asm__ volatile ("outw %1, %0" : : "dN" (port), "a" (val));
}

u8int inportb(u16int port)
{
	// allows us to read from IO device ports - also useful
	u8int ret;
	__asm__ volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

u16int inportw(u16int port)
{
	// allows us to read from IO device ports which return a 16bit int
	u16int ret;
	__asm__ volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void *memcpy(void *dest, const void *src, u32int len)
{
	// a /fast/ memcpy implementation
	char *dst = (char *)dest;
	char *sc = (char *)src;

	while (len--)
	{
		*dst++ = *sc++;
	}
	return dest;
}

void *memset(void *dest, void *val, u32int len)
{
	// a /fast/ memset implementation
	char *dst = (char *)dest;
	char *vl  = (char *)val;

	while (len--)
	{
		*dst++ = *vl;
	}
}

int strlen(char *str)
{
	char *s = str;
	int i;
	for (i = 0; *str++ != '\0';)
		i++;

	str = s;
	return i;
}
