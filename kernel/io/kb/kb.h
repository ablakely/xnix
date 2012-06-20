#ifndef KB_H
#define KB_H

int state;
void keyboard_handler(struct regs *r);
void keyboard_install();

#endif
