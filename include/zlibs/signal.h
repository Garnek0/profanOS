#ifndef _SIGNAL_H
#define _SIGNAL_H

#define SIGKILL 9
#define SIGUSR1 10

void signal(int sig, void (*handler)(int));
int kill(int pid, int sig);

#endif
