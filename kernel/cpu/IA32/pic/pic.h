#ifndef PIC_H
#define PIC_H

void pic_sendeoi(int irq);
void pic_remap(int off1, int off2);

#endif
