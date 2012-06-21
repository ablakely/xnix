#ifndef PAGING_H
#define PAGING_H

#include <iomem.h>

typedef struct page
{
	u32int present		: true;
	u32int rw		: true;
	u32int user		: true;
	u32int accessed		: true;
	u32int dirty		: true;
	u32int unused		: true;
	u32int frame		: 20;
} page_t;

typedef struct page_table
{
	page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
	page_table_t *tables[1024];
	u32int tablesPhysical[1024];
	u32int physicalAddr;
} page_directory_t;

void alloc_frame(page_t *page, bool is_kernel, bool is_writeable);
void free_frame(page_t *page);
void init_paging(void);
void switch_page_directory(page_directory_t *dir);
page_t *get_page(u32int address, int make, page_directory_t *dir);
void page_fault(struct regs *regist);

#endif
