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

#define halt() for(;;);

int xnix_main(struct multiboot *mboot_ptr)
{
	extern u32int placement_address;
	int kernel_ticks = 0;

	init_console();
	print("xnix 0.0.1 (by Aaron Blakely)\n\n");
	printf("mboot_mod_count: %x\n", mboot_ptr->mods_addr);
	init_descriptors();

	ASSERT(mboot_ptr->mods_count > 0);
	u32int initrd_location	= *((u32int*)mboot_ptr->mods_addr);
	u32int initrd_end	= *(u32int*)(mboot_ptr->mods_addr+4);

	placement_address	= initrd_end;

	__asm__ volatile("sti"); // safe to allows IRQs now.

	timer_install(50);	// set the PIT to run at 50Hz.
	keyboard_install();	// install the keboard

	init_paging();

	fs_root			= initialise_initrd(initrd_location);

	int i = 0;
	struct dirent *node = 0;
	while((node = readdir_fs(fs_root, i)) != 0)
	{
		printf("Found: %s\n", node->name);
		fs_node_t *fsnode = finddir_fs(fs_root, node->name);

		if ((fsnode->flags & 0x7) == FS_DIRECTORY)
		{
			printf("Is dir:\n");
		}
		else
		{
			printf("contents:\n");
			char buf[256];
			u32int sz = read_fs(fsnode, 0, 256, buf);
			int j;
			for (j = 0; j < sz; j++)
			{
				printf("%c", buf[j]);
			}

			print("\n");
		}
		i++;
	}


	// loop forever to keep the system alive
	for (;;)
	{
		kernel_ticks++;

	}
}
