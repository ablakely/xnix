#ifndef XNIX_H

#include <iomem.h>
#include <stdio.h>
#include <stdint.h>
#include <panic.h>
#include <boot/multiboot.h>
#include <boot/param_parser.h>
#include <tty/console.h>
#include <tty/spinner.h>
#include <tty/colors.h>
#include <cpu/IA32/IA32.h>
#include <cpu/IA32/descriptors.h>
#include <io/kb/kb.h>
#include <io/pit/pit.h>
#include <mem/paging.h>
#include <mem/malloc.h>
#include <fs/fs.h>
#include <fs/initrd.h>
#include <proc/task.h>
#include <sys/callhandler.h>
#include <io/rtc/rtc.h>

#endif
