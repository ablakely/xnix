#ifndef IA32_H
#define IA32_H

#define cpu_wait __asm__ __volatile__("outb %%al, $0x80" :: "a" (0));

#endif
