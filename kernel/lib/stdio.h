#ifndef STDIO_H
#define STDIO_H

void print(char *c);
void printc(char *c, u8int bg, u8int fg);
void print_to_point(char *text, u8int column, u8int line, u8int backColor, u8int foreColor);
void console_writehex(u32int n);
void console_writedec(u32int n);
void puts(char *c);
void error(char *c);
void printf(char const *fmt, ...);
char *sprintf(char *str, char const *fmt, ...);

#endif
