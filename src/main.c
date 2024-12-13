#include "SignalHandler.h"
#include "Server.h"

int main() {
    // Menangani sinyal SIGCHLD
    setup_sigchld_handler();

    // Mulai server pada port 8080
    start_server(8080);

    return 0;
}