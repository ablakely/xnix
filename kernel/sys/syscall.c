/* syscall.c: System Call implementations
 *
 * Written by Aaron Blakely <aaron@ephasic.org>
 */

#include "callhandler.h"
#include "syscall.h"
#include <stdint.h>
#include <stdio.h>
#include <fs/fs.h>
#include <proc/task.h>

int exit(int retval)
{
	task_exit(retval);
	while (1) { };

	return retval;
}

void *syscalls[] =
{
	(void *)&exit,
        (void *)&print,
	0
};

