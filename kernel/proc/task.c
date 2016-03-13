/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "task.h"
#include <mem/paging.h>
#include <mem/heap.h>
#include <mem/malloc.h>
#include <iomem.h>
#include <stdint.h>
#include <stdio.h>

volatile task_t *current_task;
volatile task_t *ready_queue;

extern page_directory_t		*kernel_directory;
extern page_directory_t		*current_directory;
extern void			alloc_frame(page_t*, int, int);
extern u32int			start_esp;
extern u32int			read_eip();
extern void perform_task_switch(u32int, u32int, u32int, u32int);

u32int next_pid			= 1;

void initialise_tasking()
{
	asm volatile("cli");
	move_stack((void*)0xE0000000, 0x2000);

	current_task			= ready_queue		= (task_t*)xmalloc(sizeof(task_t));
	current_task->id		= next_pid++;
	current_task->esp		= current_task->ebp	= 0;
	current_task->eip		= 0;
	current_task->page_directory	= current_directory;
	current_task->next		= 0;
	current_task->kernel_stack	= xmalloc_a(KERNEL_STACK_SIZE);

	asm volatile("sti");
	printf("Multitasking enabled.\n");
}

int fork()
{
	asm volatile("cli");
	task_t *parent_task		= (task_t*)current_task;
	page_directory_t *directory	= clone_directory(current_directory);

	task_t *new_task		= (task_t*)xmalloc(sizeof(task_t));
	new_task->id			= next_pid++;
	new_task->esp			= new_task->ebp		= 0;
	new_task->eip			= 0;
	new_task->page_directory	= directory;
	new_task->kernel_stack		= xmalloc_a(KERNEL_STACK_SIZE);
	new_task->next			= 0;

	task_t *tmp_task		= (task_t*)ready_queue;
	while (tmp_task->next)
	{
		tmp_task		= tmp_task->next;
	}

	tmp_task->next			= new_task;
	u32int eip			= read_eip();

	if (current_task == parent_task)
	{
		u32int	esp;		asm volatile("mov %%esp, %0" : "=r"(esp));
		u32int	ebp;		asm volatile("mov %%ebp, %0" : "=r"(ebp));

		new_task->esp		= esp;
		new_task->ebp		= ebp;
		new_task->eip		= eip;

		asm volatile("sti");

		return new_task->id;
	}
	else
	{
		return 0;
	}
}

void switch_task()
{
	if (!current_task)
	{
		return;
	}

	u32int esp, ebp, eip;
	asm volatile("mov %%esp, %0" : "=r"(esp));
	asm volatile("mov %%ebp, %0" : "=r"(ebp));

	eip = read_eip();
	if (eip == 0x12345)
	{
		return;
	}

	current_task->eip	= eip;
	current_task->esp	= esp;
	current_task->ebp	= ebp;

	current_task		= current_task->next;
	if (!current_task)
	{
		current_task	= ready_queue;
	}

	eip			= current_task->eip;
	esp			= current_task->esp;
	ebp			= current_task->ebp;

	current_directory = current_task->page_directory;

	set_kernel_stack(current_task->kernel_stack+KERNEL_STACK_SIZE);
	perform_task_switch(eip, current_directory->physicalAddress, ebp, esp);
}


int getpid()
{
	return current_task->id;
}

/*void task_exit(int retval)
{
	if (__builtin_expect(current_task->id == 0, 0))
	{
		switch_task();
		return;
	}

	current_task->status	= retval;
	current_task->finished	= 1;

*/

void move_stack(void *new_stack_start, u32int size)
{
	u32int i;

	for (i = (u32int)new_stack_start; i >= ((u32int)new_stack_start - size); i -= 0x1000)
	{
		alloc_frame(get_page(i, 1, current_directory), 0, 1);
	}

	// Flush the TLB
	u32int pd_addr;
	asm volatile("mov %%cr3, %0" : "=r"(pd_addr));
	asm volatile("mov %0, %%cr3" :: "r"(pd_addr));

	// old ESP and EBP
	u32int old_stack_pointer;		asm volatile("mov %%esp, %0" : "=r"(old_stack_pointer));
	u32int old_base_pointer;		asm volatile("mov %%ebp, %0" : "=r"(old_base_pointer));

	u32int offset			= (u32int)new_stack_start - start_esp;
	u32int new_stack_pointer	= old_stack_pointer + offset;
	u32int new_base_pointer		= old_base_pointer + offset;

	// Copy the stack
	memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, start_esp - old_stack_pointer);

	for (i = (u32int)new_stack_start; i > (u32int)new_stack_start - size; i -= 4)
	{
		u32int tmp = *(u32int*)i;

		if ((old_stack_pointer < tmp) && (tmp < start_esp))
		{
			tmp = tmp + offset;
			u32int *tmp2 = (u32int*)i;
			*tmp2 = tmp;
		}
	}

	// swap stacks
	asm volatile("mov %0, %%esp" :: "r"(new_stack_pointer));
	asm volatile("mov %0, %%ebp" :: "r"(new_base_pointer));
}


void switch_to_user_mode()
{
	set_kernel_stack(current_task->kernel_stack+KERNEL_STACK_SIZE);

	printf("Attempting to switch to user mode.\n");
	asm volatile("			\
		mov $0x23, %ax;		\
		mov %ax, %ds;		\
		mov %ax, %es;		\
		mov %ax, %fs;		\
		mov %ax, %gs;		\
					\
		mov %esp, %eax;		\
		push $0x23;		\
		push %eax;		\
		pushf;			\
		push $0x1B;		\
		push $1f;		\
		iret;			\
	1:				");

}
