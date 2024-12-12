#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include "HTTP_Server.h"
#include "Routes.h"
#include "Response.h"

// Semaphore untuk mengontrol jumlah proses anak
sem_t semaphore;

// Fungsi untuk log aktivitas server
void log_activity(const char *message) {
    FILE *log_file = fopen("server.log", "a");
    if (log_file) {
        fprintf(log_file, "%s\n", message);
        fclose(log_file);
    }
}

// Fungsi untuk menangani zombie processes
void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// Fungsi untuk memvalidasi URL
int validate_url(char *url) {
    if (url[0] != '/') {
        return 0; // URL harus dimulai dengan '/'
    }
    for (int i = 0; url[i] != '\0'; i++) {
        if (!(isalnum(url[i]) || url[i] == '/' || url[i] == '.' || url[i] == '-' || url[i] == '_')) {
            return 0; // Hanya karakter yang diizinkan
        }
    }
    return 1;
}

// Fungsi untuk menangani klien
void handle_client(int client_socket, struct Route *route) {
    char client_msg[4096] = "";
    if (read(client_socket, client_msg, sizeof(client_msg)-1) < 0) {
        perror("Failed to read data");
        close(client_socket);
        sem_post(&semaphore);
        return;
    }

    // Parsing HTTP request
    char *method = "";
    char *urlRoute = "";

    char *client_http_header = strtok(client_msg, "\n");

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

    char template[100] = "";

    if (!validate_url(urlRoute)) {
        strcpy(template, "templates/400.html"); // Bad request
    } else if (strcmp(method, "GET") == 0) {
        if (strstr(urlRoute, "/static/") != NULL) {
            strcat(template, "static/index.css");
        } else {
            struct Route *destination = search(route, urlRoute);
            strcat(template, "templates/");
            strcat(template, (destination == NULL) ? "404.html" : destination->value);
        }
    } else {
        strcpy(template, "templates/405.html"); // Method not allowed
    }

    char *response_data = render_static_file(template);
    char http_header[4096] = "HTTP/1.1 200 OK\r\n\r\n";
    strcat(http_header, response_data);
    strcat(http_header, "\r\n\r\n");

    // Mengirim respons ke klien
    send(client_socket, http_header, sizeof(http_header), 0);
    log_activity("Client request handled.");
    close(client_socket);
    free(response_data);
}

// Fungsi untuk menangani koneksi klien
void process_client_connection(int server_socket, struct Route *route) {
    int client_socket;
    while (1) {
        client_socket = accept(server_socket, NULL, NULL);

        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        log_activity("New client connected.");

        sem_wait(&semaphore);

        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            close(client_socket);
            sem_post(&semaphore);
            continue;
        }

        if (pid == 0) {
            close(server_socket);
            handle_client(client_socket, route);
            sem_post(&semaphore);
            exit(0);
        } else {
            close(client_socket);
        }
    }
}

int main() {
    signal(SIGCHLD, handle_sigchld);

    sem_init(&semaphore, 0, 20);  // Maksimum 20 proses anak bersamaan

    HTTP_Server http_server;
    init_server(&http_server, 6969);

    struct Route *route = initRoute("/", "index.html");
    addRoute(route, "/about", "about.html");

    process_client_connection(http_server.socket, route);

    sem_destroy(&semaphore);
    close(http_server.socket);
    return 0;
}
