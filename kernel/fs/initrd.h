#ifndef INITRD_H
#define INITRD_H

#include <stdint.h>
#include "fs.h"

typedef struct
{
	u32int nfiles;		// number of files in the ramdisk
} initrd_header_t;

typedef struct
{
	u8int	magic;		// magic number
	s8int	name[64];	// filename
	u32int	offset;		// offset in the initrd that the file starts
	u32int	length;		// length of the file
} initrd_file_header_t;

fs_node_t *initialise_initrd(u32int location);

#endif
