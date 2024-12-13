#include "HTTP_Server.h"
#include "Routes.h"
#include "ClientHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

void start_server(int port) {
    // Inisialisasi route
    struct Route *route = initialize_routes();
    
    // Inisialisasi server dan socket menggunakan init_server
    struct HTTP_Server http_server;
    init_server(&http_server, port);

    printf("Server is listening on port %d...\n", http_server.port);

    while (1) {
        int client_socket = accept(http_server.socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Failed to accept connection");
            continue;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("Failed to fork");
            close(client_socket);
            continue;
        }

        if (pid == 0) {
            // Proses anak
            close(http_server.socket); // Tutup socket server di proses anak
            handle_client(client_socket, route);
            exit(0); // Keluar dari proses anak setelah menangani klien
        } else {
            // Proses induk
            close(client_socket); // Tutup socket klien di proses induk
        }
    }

    close(http_server.socket);
}