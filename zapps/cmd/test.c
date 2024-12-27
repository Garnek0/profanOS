#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int parent_pid;
int child_pid;

// Gestionnaire pour SIGUSR1 (child)
void handler(int sig) {
    printf("Signal %d reçu par le child\n", sig);
    // Envoi du signal SIGUSR1 au parent
    kill(parent_pid, SIGUSR1);
}

// Gestionnaire pour SIGUSR1 (parent)
void handler_parent(int sig) {
    printf("Signal %d reçu par le parent\n", sig);
    // Envoi du signal SIGKILL au child
    kill(child_pid, SIGKILL);
}

int main() {
    // Récupération du PID du parent
    parent_pid = getpid();
    child_pid = fork();

    // Processus parent
    signal(SIGUSR1, handler_parent);

    // Création du processus fils
    if (child_pid == 0) {
        signal(SIGUSR1, handler);
        while (1) {
            printf("Child %d\n", getpid());    
            sleep(200);
        }
        exit(0);
    }

    sleep(1);
    
    // Envoi du signal SIGUSR1 au child
    kill(child_pid, SIGUSR1);
    
    int retstatus;
    wait(&retstatus);
    printf("Child terminated with code %d\n", WEXITSTATUS(retstatus));

    return 0;
}
