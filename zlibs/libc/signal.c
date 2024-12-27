#include <profan/syscall.h>
#include <signal.h>

void signal(int sig, void (*handler)(int)) {
    syscall_process_sigmap(syscall_process_pid(), sig, handler);
}

int kill(int pid, int sig) {
    return syscall_process_sigsend(pid, sig);
}
