#include "ClientHandler.h"
#include "Response.h"
#include "Logger.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#define MAX_CLIENT_MSG_SIZE 4096

void handle_client(int client_socket, struct Route *route) {
    char client_msg[MAX_CLIENT_MSG_SIZE] = "";
    ssize_t bytes_read = read(client_socket, client_msg, MAX_CLIENT_MSG_SIZE - 1);
    
    if (bytes_read < 0) {
        perror("Failed to read from client");
        close(client_socket);
        return;
    }else if (bytes_read > MAX_CLIENT_MSG_SIZE)
    {
            printf("Request terlalu besar, menolak permintaan.\n");
            send(client_socket, "HTTP/1.1 413 Payload Too Large\r\n\r\n", 35, 0); // 413 status code
            close(client_socket);
    }

    client_msg[bytes_read] = '\0'; // Null-terminate the string
    printf("Received message: %s\n", client_msg);

    // Parse HTTP method and route
    char method[8], urlRoute[256];
    sscanf(client_msg, "%s %s", method, urlRoute);
    printf("Method: %s, Route: %s\n", method, urlRoute);

    // Log the request
    log_request(method, urlRoute);

    if (strcmp(method, "GET") == 0) {
        // Handle GET request
        char template[100] = "templates/";
        struct Route *destination = search(route, urlRoute);
        if (destination == NULL) {
            strcat(template, "404.html");
        } else {
            strcat(template, destination->value);
        }
        char *response_data = render_static_file(template);
        send_response(client_socket, response_data ? response_data : "404 Not Found", 200);
    } else if (strcmp(method, "POST") == 0) {
        // Handle POST request (echo body)
        char *body = strstr(client_msg, "\r\n\r\n");
        if (body) {
            body += 4; // Skip header terminator
            printf("POST Body: %s\n", body);
            send_response(client_socket, body, 200);
        } else {
            send_response(client_socket, "No Body Found", 400);
        }
    } // Menangani PUT request untuk route baru
else if (strcmp(method, "PUT") == 0) {
    // Handle PUT request (add route)
    char *body = strstr(client_msg, "\r\n\r\n");
    if (body) {
        body += 4; // Skip header terminator

        // Menyusun path file dengan ekstensi yang sesuai
        char file_path[256] = "templates/";  // Folder templates, sesuaikan jika perlu
        strcat(file_path, urlRoute + 1);     // Menambahkan URL route tanpa tanda "/"
        
        // Jika file tidak memiliki ekstensi, tambahkan .html secara default
        if (strrchr(file_path, '.') == NULL) {
            strcat(file_path, ".html");
        }

        // Membuka file untuk menulis (akan membuat file baru jika tidak ada)
        FILE *file = fopen(file_path, "w");
        if (file) {
            // Menulis body dari request ke file
            fwrite(body, sizeof(char), strlen(body), file);
            fclose(file); // Menutup file setelah selesai

            // Menambahkan route ke pohon
            route = addRoute(route, urlRoute, file_path);  // Menyimpan path file di route

            send_response(client_socket, "Route added successfully and file created.", 200);
        } else {
            // Error saat membuka file
            send_response(client_socket, "Error creating file.", 500);
        }
    } else {
        send_response(client_socket, "No data to add route.", 400);
    }
    } else if (strcmp(method, "DELETE") == 0) {
        // Handle DELETE request
        struct Route *deleted = deleteRoute(route, urlRoute);
        if (deleted) {
            send_response(client_socket, "Route deleted successfully.", 200);
        } else {
            send_response(client_socket, "Route not found.", 404);
        }
    } else if (strcmp(method, "PATCH") == 0) {
        // Handle PATCH request (update route)
        char *body = strstr(client_msg, "\r\n\r\n");
        if (body) {
            body += 4;
            struct Route *existing = search(route, urlRoute);
            if (existing) {
                existing->value = body; // Replace value
                send_response(client_socket, "Route updated successfully.", 200);
            } else {
                send_response(client_socket, "Route not found.", 404);
            }
        } else {
            send_response(client_socket, "No data to update route.", 400);
        }
    } else {
        // Unknown method
        send_response(client_socket, "Method Not Allowed", 405);
    }

    close(client_socket);
}

void send_response(int client_socket, const char *body, int status_code) {
    char response[MAX_CLIENT_MSG_SIZE];
    snprintf(response, MAX_CLIENT_MSG_SIZE, "HTTP/1.1 %d OK\r\nContent-Length: %zu\r\n\r\n%s", 
             status_code, strlen(body), body);
    send(client_socket, response, strlen(response), 0);
}
