/*
 * xnix: a tiny play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "lib/console.h"

// VGA Framebuffer memory starts at 0xB8000
u16int *vram = (u16int *)0xB8000;

// see console.h for more info on this struct
struct cursor_info *cursor;

cursor->x = 0;
cursor->y = 0;
cursor->prev_x = 0;
cursor->prev_y = 0;

static void move_cursor()
{
	// screen = 80 chars wide (while in this mode)
	u16int cursorLocation = cursor.y * 80 + cursor.x;
	outportb(0x3D4, 14);		// tell the VGA board to modify the high cursor byte
	outportb(0x3D5, cursorLocation >> 8);  // set the high cursor byte
	outportb(0x3D4, 14);		// tell the VGA board to modify the low cursor byte
	outportb(0x3D5, cursorLocation);
}

void set_cursor_point(u8int x, u8int y)
{
	cursor->prev_x = cursor.x;
	cursor->prex_y = cursor.y;
	cursor->x      = x;
	cursor->y      = y;
}


