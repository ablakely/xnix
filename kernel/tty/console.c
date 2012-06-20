/*
 * xnix: a tiny play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "console.h"
#include <tty/colors.h>
#include <stdint.h>

// VA_LIST stuff
#define va_list      __builtin_va_list
#define va_start     __builtin_va_start
#define va_arg(x, y) __builtin_va_arg(x, y)
#define va_end(x)    __builtin_va_end(x)


// VGA Framebuffer memory starts at 0xB8000
u16int *vram = (u16int *)0xB8000;

// see console.h for more info on this struct
struct cursor_info *cursor;

void init_console()
{
	cursor->x = 0;
	cursor->y = 0;
	cursor->prev_x = 0;
	cursor->prev_y = 0;
	cursor->backColor = BLACK;
	cursor->foreColor = WHITE;

	clear_vram();
}
static void move_cursor()
{
	// screen = 80 chars wide (while in this mode)
	u16int cursorLocation = cursor->y * 80 + cursor->x;
	outportb(0x3D4, 14);		// tell the VGA board to modify the high cursor byte
	outportb(0x3D5, cursorLocation >> 8);  // set the high cursor byte
	outportb(0x3D4, 14);		// tell the VGA board to modify the low cursor byte
	outportb(0x3D5, cursorLocation);
}

void set_cursor_point(u8int x, u8int y)
{
	cursor->prev_x = cursor->x;
	cursor->prev_y = cursor->y;
	cursor->x      = x;
	cursor->y      = y;
}

static void scroll()
{
	int attributeByte = (0 << 4) | (15 & 0x0F);
	u16int blank	    = 0x20 | (attributeByte << 8);

	if (cursor->y >= 25)
	{
		int i;
		for (i = 80; i < 24*80; i++)
		{
			vram[i] = vram[i+80];
		}
		for (i = 24*80; i < 25*80; i++)
		{
			vram[i] = blank;
		}
		cursor->y = 24;
	}
}

void put_c(char c, u8int backColor, u8int foreColor)
{
	u8int attributeByte = (backColor << 4) | (foreColor & 0x0F);
	u16int attribute    = attributeByte << 8;
	u16int *location;

	if (c == 0x08 && cursor->x)
	{
		cursor->x--;
	}

	else if (c == 0x09)
	{
		cursor->x = (cursor->x+8) & ~(8-1);
	}

	else if (c == '\r')
	{
		cursor->x = 0;
	}

	else if (c == '\n')
	{
		cursor->x = 0;
		cursor->y++;
	}

	else if (c >= ' ')
	{
		location = vram + (cursor->y * 80 + cursor->x);
		*location = c | attribute;
		cursor->x++;
	}

	if (cursor->x >= 80)
	{
		cursor->x = 0;
		cursor->y++;
	}

	scroll();
	move_cursor();
}

void put(char c)
{
	u8int attributeByte = (cursor->backColor << 4) | (cursor->foreColor & 0x0F);
	u16int attribute    = attributeByte << 8;
	u16int *location;

	if (c == 0x08 && cursor->x)
	{
		cursor->x--;
	}

	else if (c == 0x09)
	{
		cursor->x = (cursor->x + 8) & ~(8-1);
	}

	else if (c == '\r')
	{
		cursor->x = 0;
	}

	else if (c == '\n')
	{
		cursor->x = 0;
		cursor->y++;
	}

	else if (c >= ' ')
	{
		location = vram + (cursor->y * 80 + cursor->x);
		*location = c | attribute;
		cursor->x++;
	}

	if (cursor->x >= 80)
	{
		cursor->x = 0;
		cursor->y++;
	}

	scroll();
	move_cursor();
}

void clear_vram()
{
	u8int attributeByte = (cursor->backColor << 4) | (cursor->foreColor & 0x0F);
	u16int blank = 0x20 | (attributeByte << 8);

	int i;
	for (i = 0; i < 80*25; i++)
	{
		vram[i] = blank;
	}

	cursor->x = 0;
	cursor->y = 0;
	move_cursor();
}

