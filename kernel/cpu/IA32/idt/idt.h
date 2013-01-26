#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_entry
{
	u16int	base_lo;
	u16int	sel;
	u8int	always0;
	u8int	flags;
	u16int	base_hi;
} __attribute__((packed));

struct idt_ptr
{
	u16int limit;
	u32int base;
} __attribute__((packed));

struct idt_ptr   idtp;
extern void idt_flush();

void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags);
void idt_install();

#endif
