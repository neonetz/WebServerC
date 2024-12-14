#include "SignalHandler.h"
#include "Server.h"

int main() {
    signal(SIGCHLD, handle_sigchld);


    HTTP_Server http_server;
    sem_init(&http_server.semaphore, 0, 20);  // Maksimum 20 proses anak bersamaan
    init_server(&http_server, 6969);

    struct Route *route = initRoute("/", "index.html");
    addRoute(route, "/about", "about.html");
    inorder(route);

    process_client_connection(http_server.socket, route, &http_server);

    sem_destroy(&http_server.semaphore);
    close(http_server.socket);
    return 0;
}