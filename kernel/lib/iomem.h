#ifndef IOMEM_H
#define IOMEM_H

#include <inttypes.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

void outportb(u16int port, u8int val);
void outportw(u16int port, u16int val);
u8int inportb(u16int port);
u16int inportw(u16int port);
void memcpy(void *dest, const void *src, u32int len);
void memset(void *dest, void *val, u32int len);
int strlen(char *str);

#endif IOMEM_H
