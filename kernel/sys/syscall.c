/* syscall.c: System Call implementations
 *
 * Written by Aaron Blakely <aaron@ephasic.org>
 */

#include "callhandler.h"
#include "syscall.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <fs/fs.h>
#include <proc/task.h>

 // VA_LIST stuff
#ifndef VA_LIST
#define va_list      __builtin_va_list
#define va_start     __builtin_va_start
#define va_arg(x, y) __builtin_va_arg(x, y)
#define va_end(x)    __builtin_va_end(x)
#endif


void xprint(char *c)
{
	int i = 0;
	while (c[i])
	{
		syscall_put(c[i++]);
	}
}

void xprintf(char const *fmt, ...)
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
				syscall_put('%');
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
					syscall_put('-');

				syscall_console_writedec(arg);
				state = 0;
			}
			else {
				if (chr == 'd')
				{
					syscall_put('d');
				}
				else {
					syscall_put('i');
				}
			}
		}

		else if (chr == 'c')
		{
			if (state == 1)
			{
				syscall_put((char )va_arg(args, int));
				state = 0;
			}
			else {
				syscall_put('c');
			}
		}

		else if (chr == 's')
		{
			if (state == 1)
			{
				char *temp = (char*)va_arg(args, char *);
				char *save = temp;
				xprint(temp);
				temp = save;
				state = 0;
			}
			else {
				syscall_put('s');
			}
		}

		else if (chr == 'x')
		{
			if (state == 1)
			{
				syscall_console_writehex((u32int)va_arg(args, u32int));
				state = 0;
			}
			else {
				syscall_put('x');
			}
		}

		else if (chr == 'X')
		{
			if (state == 1)
			{
				syscall_console_writehex((u32int)va_arg(args, u32int));
				state = 0;
			}
			else {
				syscall_put('X');
			}
		}

		else if (chr == '?')
		{
			if (state == 1)
			{
				char *temp__ = (char *)va_arg(args, char *);
				if ((bool)va_arg(args, bool))
				{
					xprint(temp__);
				}
				state = 0;
			}
			else {
				syscall_put('?');
			}
		}

		else if (chr == 'f')
		{
			if (state == 1)
			{
				double tmp__ = va_arg(args, double);
				char *tmmp;
				xprint(dtostr(tmp__, tmmp, 0));
			}
			else {
				syscall_put('f');
			}
		}
		else {
			syscall_put(chr);
		}
	}
	va_end(args);
}
