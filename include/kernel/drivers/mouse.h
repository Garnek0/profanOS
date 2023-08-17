#ifndef MOUSE_H
#define MOUSE_H

#include <cpu/isr.h>
#include <type.h>

int mouse_init();
void mouse_handler(registers_t *a_r);

int mouse_call(int thing, int val);

#endif
