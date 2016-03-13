/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "heap.h"
#include "ordered_array.h"
#include "malloc.h"
#include "paging.h"
#include <stdint.h>
#include <panic.h>

heap_t *xnix_heap	= 0;

s32int find_smallest_hole(u32int size, u8int page_align, heap_t *heap)
{
	u32int iterator = 0;
	while (iterator < heap->index.size)
	{
		header_t *header	= (header_t *)lookup_ordered_array(iterator, &heap->index);
		if (page_align > 0)
		{
			u32int location		= (u32int)header;
			s32int offset		= 0;

			if (((location+sizeof(header_t)) & 0xFFFFF000) != 0)
			{
				offset = 0x1000 - (location+sizeof(header_t))%0x1000;
			}

			s32int hole_size	= (s32int)header->size - offset;

			if (hole_size >= (s32int)size)
			{
				break;
			}
		}
		else if (header->size >= size)
		{
			break;
		}

		iterator++;
	}

	if (iterator == heap->index.size)
	{
		return -1;
	}
	else
	{
		return iterator;
	}
}

s8int header_t_less_than(void *a, void *b)
{
	return (((header_t*)a)->size < ((header_t*)b)->size)?1:0;
}

heap_t *create_heap(u32int start, u32int end_addr, u32int max, u8int supervisor, u8int readonly)
{
	heap_t *heap		= (heap_t*)xmalloc(sizeof(heap_t));

	ASSERT(start%0x1000 == 0);
	ASSERT(end_addr%0x1000 == 0);

	ordered_array_t temp	= place_ordered_array((void*)start, XNIX_HEAP_INDEX_SIZE, &header_t_less_than);
	heap->index		= temp;

	start		       += sizeof(type_t)*XNIX_HEAP_INDEX_SIZE;

	if (start & 0xFFFFF000 != 0)
	{
		start	       &= 0xFFFFF000;
		start	       += 0x1000;
	}

	heap->start_address	= start;
	heap->end_address	= end_addr;
	heap->max_address	= max;
	heap->supervisor	= supervisor;
	heap->readonly		= readonly;

	header_t *hole		= (header_t *)start;
	hole->size		= end_addr - start;
	hole->magic		= XNIX_HEAP_MAGIC;
	hole->is_hole		= 1;

	insert_ordered_array((void*)hole, &heap->index);

	return heap;
}

void expand(u32int new_size, heap_t *heap)
{
	ASSERT(new_size > heap->end_address - heap->start_address);

	if (new_size & 0xFFFFF000 != 0)
	{
		new_size	&= 0xFFFFF000;
		new_size	+= 0x1000;
	}

	ASSERT(heap->start_address + new_size <= heap->max_address);

	u32int old_size		= heap->end_address - heap->start_address;
	u32int i		= old_size;

	while (i < new_size)
	{
		alloc_frame(get_page(heap->start_address + i, 1, kernel_directory), (heap->supervisor)?1:0, (heap->readonly)?0:1);
		i += 0x1000;
	}

	heap->end_address	= heap->start_address + new_size;
}

u32int contract(u32int new_size, heap_t *heap)
{
	ASSERT(new_size < heap->end_address - heap->start_address);

	if (new_size & 0x1000)
	{
		new_size &= 0x1000;
		new_size += 0x1000;
	}

	if (new_size < XNIX_HEAP_MIN_SIZE)
	{
		new_size = XNIX_HEAP_MIN_SIZE;
	}

	u32int old_size		= heap->end_address - heap->start_address;
	u32int i		= old_size - 0x1000;

	while (new_size < i)
	{
		free_frame(get_page(heap->start_address + i, 0, kernel_directory));
		i -= 0x1000;
	}

	heap->end_address	= heap->start_address + new_size;
	return new_size;
}

void *alloc(u32int size, u8int page_align, heap_t *heap)
{
	u32int new_size		= size + sizeof(header_t) + sizeof(footer_t);
	u32int iterator		= find_smallest_hole(new_size, page_align, heap);

	if (iterator == -1)
	{
		u32int old_length	= heap->end_address - heap->start_address;
		u32int old_end_address	= heap->end_address;

		expand(old_length+new_size, heap);
		u32int new_length	= heap->end_address - heap->start_address;

		iterator		= 0;
		u32int idx		= -1;
		u32int value		= 0x0;

		while (iterator < heap->index.size)
		{
			u32int tmp	= (u32int)lookup_ordered_array(iterator, &heap->index);
			if (tmp > value)
			{
				value	= tmp;
				idx	= iterator;
			}
			iterator++;
		}

		if (idx == -1)
		{
			header_t *header	= (header_t *)old_end_address;
			header->magic		= XNIX_HEAP_MAGIC;
			header->size		= new_length - old_length;
			header->is_hole		= 1;

			footer_t *footer	= (footer_t *)(old_end_address + header->size - sizeof(footer_t));
			footer->magic		= XNIX_HEAP_MAGIC;
			footer->header		= header;

			insert_ordered_array((void*)header, &heap->index);
		}
		else
		{
			header_t *header	 = lookup_ordered_array(idx, &heap->index);
			header->size		+= new_length - old_length;

			footer_t *footer	 = (footer_t *)((u32int)header + header->size - sizeof(footer_t));
			footer->header		 = header;
			footer->magic		 = XNIX_HEAP_MAGIC;
		}

		return alloc(size, page_align, heap);
	}

	header_t *orig_hole_header	= (header_t *)lookup_ordered_array(iterator, &heap->index);

	u32int orig_hole_pos		= (u32int)orig_hole_header;
	u32int orig_hole_size		= orig_hole_header->size;

	if (orig_hole_size - new_size < sizeof(header_t) + sizeof(footer_t))
	{
		size			+= orig_hole_size - new_size;
		new_size		 = orig_hole_size;
	}

	if (page_align && orig_hole_pos & 0xFFFFF000)
	{
		u32int new_location	= orig_hole_pos + 0x1000 - (orig_hole_pos & 0xFFF) - sizeof(header_t);

		header_t *hole_header	= (header_t *)orig_hole_pos;
		hole_header->size	= 0x1000 - (orig_hole_pos & 0xFFF) - sizeof(header_t);
		hole_header->magic	= XNIX_HEAP_MAGIC;
		hole_header->is_hole	= 1;

		footer_t *hole_footer	= (footer_t *)((u32int)new_location - sizeof(footer_t));
		hole_footer->magic	= XNIX_HEAP_MAGIC;
		hole_footer->header	= hole_header;

		orig_hole_pos		= new_location;
		orig_hole_size		= orig_hole_size - hole_header->size;
	}
	else
	{
		remove_ordered_array(iterator, &heap->index);
	}

	header_t *block_header		= (header_t *)orig_hole_pos;
	block_header->magic		= XNIX_HEAP_MAGIC;
	block_header->is_hole		= 0;
	block_header->size		= new_size;

	footer_t *block_footer		= (footer_t *)(orig_hole_pos + sizeof(header_t) + size);
	block_footer->magic		= XNIX_HEAP_MAGIC;
	block_footer->header		= block_header;

	if (orig_hole_size - new_size > 0)
	{
		header_t *hole_header	= (header_t *)(orig_hole_pos + sizeof(header_t) + size + sizeof(footer_t));
		hole_header->magic	= XNIX_HEAP_MAGIC;
		hole_header->is_hole	= 1;
		hole_header->size	= orig_hole_size - new_size;

		footer_t *hole_footer	= (footer_t *)( (u32int)hole_header + orig_hole_size - new_size - sizeof(footer_t));
		if (((u32int)hole_footer + sizeof(footer_t)) <= heap->end_address)
		{
			hole_footer->magic	= XNIX_HEAP_MAGIC;
			hole_footer->header	= hole_header;
		}

		insert_ordered_array((void*)hole_header, &heap->index);
	}

	return (void*)((u32int)block_header+sizeof(header_t));
}

void free(void *p, heap_t *heap)
{
	if (p == 0)
	{
		return;
	}

	header_t *header		= (header_t *)((u32int)p - sizeof(header_t));
	footer_t *footer		= (footer_t *)((u32int)header + header->size - sizeof(footer_t));

	ASSERT(header->magic == XNIX_HEAP_MAGIC);
	ASSERT(footer->magic == XNIX_HEAP_MAGIC);

	header->is_hole			= 1;
	char do_add			= 1;

	footer_t *test_footer		= (footer_t *)((u32int)header - sizeof(footer_t));

	if (test_footer->magic == XNIX_HEAP_MAGIC && test_footer->header->is_hole == 1)
	{
		u32int cache_size	 = header->size;		// cache our current size
		header			 = test_footer->header;		// rewrite header with new one
		footer->header		 = header;
		header->size		+= cache_size;			// change size
		do_add			 = 0;				// don't add to the index again
	}

	header_t *test_header		= (header_t *)((u32int)footer + sizeof(footer_t));

	if (test_header->magic == XNIX_HEAP_MAGIC && test_header->is_hole)
	{
		header->size		+= test_header->size;
		test_footer		 = (footer_t*)((u32int)test_header + test_header->size - sizeof(footer_t));
		footer			 = test_footer;

		u32int iterator		= 0;
		while((iterator < heap->index.size) && (lookup_ordered_array(iterator, &heap->index) != (void*)test_header))
		{
			iterator++;
		}

		ASSERT(iterator < heap->index.size);
		remove_ordered_array(iterator, &heap->index);
	}

	if ((u32int)footer+sizeof(footer_t) == heap->end_address)
	{
		u32int old_length	= heap->end_address - heap->start_address;
		u32int new_length 	= contract((u32int)header - heap->start_address, heap);

		if (header->size - (old_length - new_length) > 0)
		{
			header->size	-= old_length - new_length;
			footer		 = (footer_t*)((u32int)header + header->size - sizeof(footer_t));
			footer->magic	 = XNIX_HEAP_MAGIC;
			footer->header	 = header;
		}
		else
		{
			u32int iterator	= 0;
			while((iterator < heap->index.size) && (lookup_ordered_array(iterator, &heap->index) != (void*)test_header))
			{
				iterator++;
			}

			if (iterator < heap->index.size)
			{
				remove_ordered_array(iterator, &heap->index);
			}
		}
	}

	if (do_add == 1)
	{
		insert_ordered_array((void*) header, &heap->index);
	}
}
