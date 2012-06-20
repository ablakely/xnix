/*
 * xnix: a tiny play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include <iomem.h>
#include <tty/console.h>
#include <tty/colors.h>

// VA_LIST stuff
#ifndef VA_LIST
#define va_list      __builtin_va_list
#define va_start     __builtin_va_start
#define va_arg(x, y) __builtin_va_arg(x, y)
#define va_end(x)    __builtin_va_end(x)
#endif

void print(char *c)
{
	int i = 0;
	while (c[i])
	{
		put(c[i++]);
	}
}

void printc(char *c, u8int bg, u8int fg)
{
	int i = 0;
	while (c[i])
	{
		put_c(c[i++], bg, fg);
	}
}

void error(char *c)
{
	printc(c, BLACK, LIGHT_MAGENTA);
}

void printf(char const *fmt, ...)
{
	va_list args;
	va_start(args, *fmt);
	int i;
	int state = 0;

	for (i = 0; fmt[i] != 0; i++)
	{
		char chr = fmt[i];
		if (chr == '%')
		{
			if (state == 1)
			{
				put('%');
				state = 0;
			}
			else {
				state++;
			}
		}
		else if (chr == 'd' || chr == 'i')
		{
			if (state == 1)
			{
				u32int arg = (u32int)va_arg(args, int);
				if (arg < 0)
					put('-');

				console_writedec(arg);
				state = 0;
			}
			else {
				if (chr == 'd')
				{
					put('d');
				}
				else {
					put('i');
				}
			}
		}
		else if (chr == 'c')
		{
			if (state == 1)
			{
				put((char )va_arg(args, int));
				state = 0;
			}
			else {
				put('c');
			}
		}
		else if (chr == 's')
		{
			if (state == 1)
			{
				char *temp = (char*)va_arg(args, char *);
				char *save = temp;
				print(temp);
				temp = save;
				state = 0;
			}
			else {
				put('s');
			}
		}
		else if (chr == 'x')
		{
			if (state == 1)
			{
				console_writehex((u32int)va_arg(args, u32int));
				state = 0;
			}
			else {
				put('x');
			}
		}
		else if (chr == 'X')
		{
			if (state == 1)
			{
				console_writehex((u32int)va_arg(args, u32int));
				state = 0;
			}
			else {
				put('X');
			}
		}
		else if (chr == '?')
		{
			if (state == 1)
			{
				char *temp__ = (char *)va_arg(args, char *);
				if ((bool)va_arg(args, bool))
				{
					print(temp__);
				}
				state = 0;
			}
			else {
				put('?');
			}
		}
		else if (chr == 'f')
		{
			if (state == 1)
			{
				double tmp__ = va_arg(args, double);
				char *tmmp;
				print(dtostr(tmp__, tmmp, 0));
			}
			else {
				put('f');
			}
		}
		else {
			put(chr);
		}
	}
	va_end(args);
}

char *sprintf(char *str, char const *fmt, ...)
{
	char *save = str;
	va_list args;
	va_start(args, *fmt);
	static int state;
	for (; *fmt != 0; *fmt++)
	{
		switch (*fmt)
		{
			case '%':
				if (state == 1)
				{
					*str++ ='%';
					state--;
				}
				else {
					state++;
				}
			break;

			case 's':
				if (state == 1)
				{
					char *tmp = va_arg(args, char *);
					char *save = tmp;
					while ((*str++ = *tmp++) != 0);
					tmp = save;
				}
				else {
					*str++ = 's';
				}
			break;

			case 'd':
				if (state == 1)
				{
					char *temp;
					itoa(va_arg(args, int), temp, 10);
					while (temp != '\0') *str++ = *temp++;
				}
				else {
					*str++ = 'd';
				}
			break;

			case 'x':
				if (state == 1)
				{
					char *temp;
					itoa(va_arg(args, int), temp, 16);
					while (temp != '\0') *str++ = *temp++;
				}
				else {
					*str++ = 'x';
				}
			break;

			case 'c':
				if (state == 1)
				{
					*str++ = va_arg(args, int);
				}
				else {
					*str++ = 'c';
				}
			break;

			default:
				*str++ = *fmt++;
		}
	}
	str = '\0';
	va_end(args);
	return save;
}
