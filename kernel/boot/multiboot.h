#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

struct multiboot_info {
	u32int flags;
	u32int memory_lo;
	u32int memory_hi;
	u32int bootDevice;
	u32int cmdLine;
	u32int mods_count;
	u32int mods_addr;
	u32int syms0;
	u32int syms1;
	u32int syms2;
	u32int mmap_length;
	u32int mmap_addr;
	u32int drives_length;
	u32int drives_addr;
	u32int config_table;
	u32int apm_table;
	u32int vbe_control_ingo;
	u32int vbe_mode_info;
	u32int vbe_mode;
	u32int vbe_interface_seg;
	u32int vbe_interface_off;
	u32int vbe_interface_len;
};

#endif
