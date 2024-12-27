#ifndef SIGNAL_H
#define SIGNAL_H

#include <system.h>

extern void *sig_default[];
void sig_call(void *func);

#endif
