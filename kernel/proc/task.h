#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <mem/paging.h>

#define KERNEL_STACK_SIZE 2048

typedef struct task
{
	int			id;			// Process Id
	u32int			esp, ebp;		// Stack and base pointer
	u32int			eip;			// Instruction pointer
	page_directory_t	*page_directory;	// Page directory
	u32int			kernel_stack;
	struct task		*next;			// next task in linked list
} task_t;

void 	initialise_tasking();
void 	task_switch();
int 	fork();
void 	move_stack(void *new_stack_start, u32int size);
int 	getpid();
void	switch_to_user_mode();


#endif
