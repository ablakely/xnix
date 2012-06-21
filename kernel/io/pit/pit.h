#ifndef PIT_H
#define PIT_H

#include <iomem.h>

int uptime;

void timer_phase(int hz);
void timer_handler(struct regs *r);
void timer_install();
void timer_wait(int ticks);

#endif
