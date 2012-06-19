#ifndef DETECT_H
#define DETECT_H

#include <inttypes.h>

typedef struct SMAP_entry {
	u32int	BaseL;
	u32int	BaseH;
	u32int	LengthL;
	u32int	LengthH;
	u16int	Type;
	u16int	ACPI;
} __attribute__((packed)) SMAP_entry_t;

#endif DETECT_H
