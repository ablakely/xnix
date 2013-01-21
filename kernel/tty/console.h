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

void init_console();
void move_cursor();
void set_cursor_point(u8int x, u8int y);
void scroll();
void print_to_point(char *text, u8int column, u8int line, u8int backColor, u8int foreColor);
void printc_to_point(char c, u8int column, u8int line, u8int backColor, u8int foreColor);
void console_writehex(u32int n);
void console_writedec(u32int n);
void put_c(char c, u8int backColor, u8int foreColor);
void put(char c);
void clear_vram();

#endif
