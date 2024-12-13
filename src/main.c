#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include "HTTP_Server.h"
#include "Routes.h"
#include "Response.h"

#define MAX_CLIENT_MSG_SIZE 4096

void log_request(const char *method, const char *url) {
    FILE *log_file = fopen("server.log", "a");
    if (log_file != NULL) {
        time_t now = time(NULL);
        char *time_str = ctime(&now);
        time_str[strlen(time_str) - 1] = '\0'; // Menghapus newline di akhir

        fprintf(log_file, "[%s] Method: %s, URL: %s\n", time_str, method, url);
        fclose(log_file);
    } else {
        perror("Failed to open log file");
    }
}

// Handler untuk SIGCHLD
void sigchld_handler(int signo) {
    // Menghapus semua proses anak yang telah selesai
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void handle_client(int client_socket, struct Route *route) {
    char client_msg[MAX_CLIENT_MSG_SIZE] = "";
    ssize_t bytes_read = read(client_socket, client_msg, MAX_CLIENT_MSG_SIZE - 1);
    
    if (bytes_read < 0) {
        perror("Failed to read from client");
        close(client_socket);
        return;
    }
    
    client_msg[bytes_read] = '\0'; // Null-terminate the string
    printf("Received message: %s\n", client_msg);

    // Cek apakah ini adalah permintaan HTTP
    if (strncmp(client_msg, "GET", 3) == 0 || strncmp(client_msg, "POST", 4) == 0) {
        // Parsing client socket header to get HTTP method, route
        char *method = "";
        char *urlRoute = "";

        char *client_http_header = strtok(client_msg, "\n");
        
        printf("\n\n%s\n\n", client_http_header);

        char *header_token = strtok(client_http_header, " ");
        
        int header_parse_counter = 0;

        while (header_token != NULL) {
            switch (header_parse_counter) {
                case 0:
                    method = header_token;
                    break;
                case 1:
                    urlRoute = header_token;
                    break;
            }
            header_token = strtok(NULL, " ");
            header_parse_counter++;
        }

        printf("The method is %s\n", method);
        printf("The route is %s\n", urlRoute);

        // Log the request
        log_request(method, urlRoute);

        char template[100] = "";
        
        if (strstr(urlRoute, "/static/") != NULL) {
            strcat(template, "static/index.css");
        } else {
            struct Route * destination = search(route, urlRoute);
            strcat(template, "templates/");

            if (destination == NULL) {
                strcat(template, "404.html");
            } else {
                strcat(template, destination->value);
            }
        }

        char * response_data = render_static_file(template);

        char http_header[4096] = "HTTP/1.1 200 OK\r\n\r\n";

        if (response_data != NULL) {
            strcat(http_header, response_data);
        } else {
            strcat(http_header, "404 Not Found");
        }
        
        strcat(http_header, "\r\n\r\n");

        send(client_socket, http_header, strlen(http_header), 0);
    } else {
        // Jika bukan permintaan HTTP, anggap sebagai pesan echo
        ssize_t bytes_sent = send(client_socket, client_msg, bytes_read, 0);
        if (bytes_sent < 0) {
            perror("Failed to send message to client");
        }
    }

    close(client_socket);
}

int main() {
    // Menangani sinyal SIGCHLD
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);

    // Inisialisasi route
        struct Route *route = initialize_routes();
    
        // Inisialisasi server dan socket
    struct HTTP_Server http_server;
    http_server.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (http_server.socket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(http_server.socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind socket");
        close(http_server.socket);
        exit(EXIT_FAILURE);
    }

    listen(http_server.socket, 5);
    printf("Server is listening on port 8080...\n");

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
    return 0;
}