#ifndef IOMEM_H
#define IOMEM_H

#include <stdint.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

struct regs
{
	unsigned int gs, fs, ex, ds;
	unsigned int edi, esi, ebp, ebx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};

void outportb(u16int port, u8int val);
void outportw(u16int port, u16int val);
u8int inportb(u16int port);
u16int inportw(u16int port);
void *memcpy(void *dest, const void *src, u32int len);
void *memset(void *dest, void *val, u32int len);
int strlen(char *str);

#endif 
