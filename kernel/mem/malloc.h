#ifndef MALLOC_H
#define MALLOC_H

#include <stdint.h>

u32int xmalloc_int(u32int sz, int align, u32int *phys);
u32int xmalloc(u32int sz);
u32int xmalloc_(u32int sz, u32int *phys);
u32int xmalloc_p(u32int sz, u32int *phys);
u32int malloc(u32int sz);

#endif
