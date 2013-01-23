#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#include <stdint.h>

struct tss_entry_struct
{
        u32int          prev_tss;               // previous TSS - if used hardware task switching this would form a linked list
        u32int          esp0;                   // stack pointer to load when change to kernel mode
        u32int          ss0;                    // stack segment to load when change to kernel mode

        u32int          esp1;
        u32int          ss1;
        u32int          esp2;
        u32int          ss2;
	u32int		cr3;
        u32int          eip;
        u32int          eflags;
        u32int          eax;
        u32int          ecx;
        u32int          edx;
        u32int          ebx;
        u32int          esp;
        u32int          ebp;
        u32int          esi;
        u32int          edi;
        u32int          es;                     // value to load into ES when change to kernel mode
        u32int          cs;                     // value to load into CS when change to kernel mode
        u32int          ss;                     // value to load into SS when change to kernel mode
        u32int          ds;                     // value to load into DS when change to kernel mode
        u32int          fs;                     // value to load into FS when change to kernel mode
        u32int          gs;                     // value to load into GS when change to kernel mode
        u32int          ldt;
        u16int          trap;
        u16int          iomap_base;
} __attribute__((packed));

typedef struct tss_entry_struct tss_entry_t;


void init_descriptors();

#endif
