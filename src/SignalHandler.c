#define _GNU_SOURCE
#include "SignalHandler.h"
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief Signal handler for SIGCHLD.
 *
 * This function is called when a SIGCHLD signal is received, indicating that a child process has terminated.
 * It reaps all terminated child processes to prevent zombie processes by calling waitpid in a loop with WNOHANG.
 *
 * @param signo The signal number (should be SIGCHLD).
 */
void sigchld_handler(int signo) {
    // Menghapus semua proses anak yang telah selesai
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

/**
 * @brief Sets up the signal handler for SIGCHLD.
 *
 * This function configures the signal handler for the SIGCHLD signal, which is sent to a parent process
 * when a child process terminates. The handler is set to `sigchld_handler`, and the signal action is 
 * configured to restart interrupted system calls and to not generate a signal when child processes stop.
 *
 * The `sigaction` structure is initialized with the following settings:
 * - `sa_handler`: The signal handler function (`sigchld_handler`).
 * - `sa_mask`: An empty signal set, meaning no additional signals are blocked during the execution of the handler.
 * - `sa_flags`: The flags `SA_RESTART` and `SA_NOCLDSTOP` are set.
 *   - `SA_RESTART`: Ensures that certain system calls are restarted if interrupted by this signal.
 *   - `SA_NOCLDSTOP`: Prevents the generation of SIGCHLD when child processes stop.
 *
 * @note This function should be called during the initialization phase of the program to ensure that
 *       the signal handler is set up before any child processes are created.
 */
void setup_sigchld_handler(void) {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
}