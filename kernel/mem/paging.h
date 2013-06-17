#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <iomem.h>
#include <cpu/IA32/IA32.h>

u32int start_esp;

typedef struct page
{
	u32int present		: 1;
	u32int rw		: 1;
	u32int user		: 1;
	u32int accessed		: 1;
	u32int dirty		: 1;
	u32int unused		: 7;
	u32int frame		: 20;
} page_t;

typedef struct page_table
{
	page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
	page_table_t  *tables[1024];
	u32int tablesPhysical[1024];
	u32int physicalAddress;
} page_directory_t;

page_directory_t *kernel_directory;
page_directory_t *current_directory;

void init_paging();
void switch_page_directory(page_directory_t *new);
page_t *get_page(u32int address, int make, page_directory_t *dir);
void page_fault(struct regs *r);

page_directory_t *clone_directory(page_directory_t *src);
page_table_t *clone_table(page_table_t *src, u32int *physAddr);

#endif
