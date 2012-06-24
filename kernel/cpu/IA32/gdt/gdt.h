#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct gdt_entry
{
        u16int limit_low;
        u16int base_low;
        u8int  base_middle;
        u8int  access;
        u8int  granularity;
        u8int  base_high;
} __attribute__((packed));

struct gdt_ptr
{
        u16int limit;
        u32int base;
} __attribute__((packed));

void gdt_set_gate(int num, u32int base, u32int limit, u8int access, u8int gran);
void gdt_install();

#endif

