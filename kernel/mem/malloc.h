#ifndef MALLOC_H
#define MALLOC_H
#include <stdint.h>

u32int inital_esp;
u32int placement_address;

u32int xmalloc_int(u32int sz, int align, u32int *phys);
u32int xmalloc_a(u32int sz);
u32int xmalloc_p(u32int sz, u32int *phys);
u32int xmalloc_ap(u32int sz, u32int *phys);
u32int xmalloc(u32int sz);
void xfree(void *p);

#endif
