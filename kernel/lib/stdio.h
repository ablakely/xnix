#ifndef STDIO_H
#define STDIO_H

#include "stdint.h"

void print(char *c);
void printc(char *c, u8int bg, u8int fg);
void puts(char *c);
void error(char *c);
void printf(char const *fmt, ...);
char *sprintf(char *str, char const *fmt, ...);

#endif
