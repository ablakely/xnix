#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

u32int page_aligned_end;
u32int *page_directory;

u32int *create_page_table(int page_entry);
void init_paging();

#endif
