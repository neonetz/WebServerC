#include "HTTP_Server.h"
#include "Routes.h"
#include "ClientHandler.h"
#include "Logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>

/**
 * @brief Starts the HTTP server on the specified port.
 *
 * This function initializes the server, sets up the routes, and listens for incoming client connections.
 * For each client connection, it forks a new process to handle the client request.
 *
 * @param port The port number on which the server will listen for incoming connections.
 *
 * The function performs the following steps:
 * 1. Initializes the routes using `initialize_routes()`.
 * 2. Initializes the server and socket using `init_server()`.
 * 3. Enters an infinite loop to listen for incoming client connections.
 * 4. For each client connection:
 *    - Accepts the connection.
 *    - Forks a new process to handle the client request.
 *    - In the child process:
 *      - Closes the server socket.
 *      - Handles the client request using `handle_client()`.
 *      - Exits the child process after handling the client.
 *    - In the parent process:
 *      - Waits for the child process to terminate.
 *      - Closes the client socket.
 * 5. Closes the server socket when the server is stopped.
 *
 * Error handling:
 * - If accepting a connection fails, an error message is printed and the server continues to listen for new connections.
 * - If forking a new process fails, an error message is printed, the client socket is closed, and the server continues to listen for new connections.
 */
void start_server(int port) {
    // Inisialisasi route
    struct Route *route = initialize_routes();
    
    // Inisialisasi server dan socket menggunakan init_server
    struct HTTP_Server http_server;
    init_server(&http_server, port);

    printf("Server is listening on port %d...\n", http_server.port);

    while (1) {

        pid_t pid = getpid();
        int client_socket = accept(http_server.socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Failed to accept connection");
            continue;
        }else{
            pid = fork();
            log_fork("[00]client fork", pid);
        }

        if (pid < 0) {
            perror("Failed to fork");
            close(client_socket);
            exit(EXIT_FAILURE);
            continue;
        }

        if (pid == 0) {
            // Proses anak
            log_fork("[00]client fork", getpid());
            close(http_server.socket); // Tutup socket server di proses anak
            handle_client(client_socket, route);
            exit(EXIT_SUCCESS); // Keluar dari proses anak setelah menangani klien
        } else {
            // Proses induk
            int status;
            if (waitpid(pid, &status, WNOHANG) < 0)
            {
                log_fork("[01]terminate fork", pid);
                pid = fork();
                if (pid == 0)
                {
                    log_fork("[02]replace fork", getpid());
                    /* code */
                }
                
            }
            close(client_socket); // Tutup socket klien di proses induk
        }
    }
    close(http_server.socket);
}