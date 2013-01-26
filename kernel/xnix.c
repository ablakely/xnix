/*
 * xnix: small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include <iomem.h>
#include <stdio.h>
#include <stdint.h>
#include <panic.h>
#include <boot/multiboot.h>
#include <tty/console.h>
#include <tty/spinner.h>
#include <tty/colors.h>
#include <cpu/IA32/IA32.h>
#include <cpu/IA32/descriptors.h>
#include <io/kb/kb.h>
#include <io/pit/pit.h>
#include <mem/paging.h>
#include <mem/malloc.h>
#include <fs/fs.h>
#include <fs/initrd.h>
#include <proc/task.h>
#include <syscall.h>

#define halt() for(;;);
extern u32int  start_esp;

int xnix_main(struct multiboot *mboot_ptr, u32int initial_stack)
{
	extern u32int placement_address;
	int kernel_ticks 	= 0;
	start_esp		= initial_stack;

	init_console();
	print("xnix 0.0.1 (by Aaron Blakely)\n\n");

	init_descriptors();

	ASSERT(mboot_ptr->mods_count > 0);
	u32int initrd_location	= *((u32int*)mboot_ptr->mods_addr);
	u32int initrd_end	= *(u32int*)(mboot_ptr->mods_addr+4);

	placement_address	= initrd_end;

	timer_install(50);	// set the PIT to run at 50Hz.
	keyboard_install();	// install the keboard

	asm volatile("sti");	// re-enable interrupts

	init_paging();
	initialise_tasking();

	fs_root			= initialise_initrd(initrd_location);

	init_syscalls();

	switch_to_user_mode();

	syscall_print("Hello world!\n");

	// loop forever to keep the system alive
	for (;;)
	{
		kernel_ticks++;

	}
}
