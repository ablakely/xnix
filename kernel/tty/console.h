#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

struct cursor_info {
	u8int x;
	u8int y;
	u8int prev_x;
	u8int prev_y;
	u8int backColor;
	u8int foreColor;
};

static void move_cursor();
void set_cursor_point(u8int x, u8int y);
static void scroll();
void put_c(char c, u8int backColor, u8int foreColor);
void put(char c);
void clear_vram();

#endif
