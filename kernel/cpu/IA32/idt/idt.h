#ifndef IDT_H
#define IDT_H

struct idt_entry
{
	unsigned short base_lo;
	unsigned short sel;
	unsigned char  always0;
	unsigned char  flags;
	unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
	unsigned short limit;
	unsigned int   base;
} __attribute__((packed));

extern void idt_load();

#endif
