#define _GNU_SOURCE
#include "SignalHandler.h"
#include <sys/wait.h>
#include <unistd.h>

void sigchld_handler(int signo) {
    // Menghapus semua proses anak yang telah selesai
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void setup_sigchld_handler(void) {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
}