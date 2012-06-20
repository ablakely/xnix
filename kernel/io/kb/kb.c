/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasic.org>
 *
 */

#include "kb.h"
#include "map.h"
#include "buffer.h"
#include <iomem.h>
#include <cpu/IA32/irq.h>
#include <tty/console.h>
#include <tty/color.h>

state = 0;
