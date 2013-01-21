#ifndef TSS_H
#define TSS_H

#include <stdint.h>

void write_tss(s32int num, u16int ss0, u32int esp0);
void set_kernel_stack(u32int stack);

#endif
