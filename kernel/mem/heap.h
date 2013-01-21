#ifndef HEAP_H
#define HEAP_H

#include "ordered_array.h"
#include <stdint.h>

#define XNIX_HEAP_START		0xC0000000
#define XNIX_HEAP_INITIAL_SIZE	0x100000
#define XNIX_HEAP_INDEX_SIZE	0x20000
#define XNIX_HEAP_MAGIC		0x123890AB
#define XNIX_HEAP_MIN_SIZE	0x70000

typedef struct
{
	u32int magic;		// Magic Number, used for error checking and identification
	u32int is_hole;		// 1 if hole, 0 if block
	u32int size;		// size of the block, including the end footer.
} header_t;

typedef struct
{
	u32int magic;		// Magic number, same as in header_t
	header_t *header;	// pointer to the block header
} footer_t;

typedef struct
{
	ordered_array_t	index;
	u32int		start_address;		// start of allocated space
	u32int		end_address;		// end of allocated space.  May be expanded up to max_address
	u32int		max_address;		// max address the heap can be expanded to
	u8int		supervisor;		// should extra pages requested be mapped to supervisor-only?
	u8int		readonly;		// should extra pages requested be mapped as read-only?
} heap_t;

heap_t *xnix_heap;

heap_t *create_heap(u32int start, u32int end, u32int max, u8int supervisor, u8int readonly);
void *alloc(u32int size, u8int page_align, heap_t *heap);
void free(void *p, heap_t *heap);

#endif
