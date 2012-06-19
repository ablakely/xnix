#ifndef IOMEM_H
#define IOMEM_H

// some typedefs to make things a little more sane
// written on X86 (32bit)

typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

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
