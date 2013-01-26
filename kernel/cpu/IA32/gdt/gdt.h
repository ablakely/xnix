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

typedef struct gdt_entry gdt_entry_t;
typedef struct gdt_ptr   gdt_ptr_t;

void gdt_set_gate(u32int num, u32int base, u32int limit, u8int access, u8int gran);
void gdt_install();

#endif

