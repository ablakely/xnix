#ifndef IA32_H
#define IA32_H

#define cpu_wait() __asm__ __volatile__("outb %%al, $0x80" :: "a" (0));

typedef struct regs
{
	u32int	ds;	// TODO: gs, es, fs? 
	u32int	edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32int	int_no, err_code;
	u32int	eip, cs, eflags, useresp, ss;
} registers_t;

#endif
