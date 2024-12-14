#include "ClientHandler.h"
#include "Response.h"
#include "Logger.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#define MAX_CLIENT_MSG_SIZE 4096

// Fungsi untuk menghapus file
int deleteFile(const char *filePath)
{
    if (unlink(filePath) == 0)
    {
        return 1; // File berhasil dihapus
    }
    else
    {
        perror("Gagal menghapus file");
        return 0; // File gagal dihapus
    }
}

void handle_client(int client_socket, struct Route *route) {
    char client_msg[MAX_CLIENT_MSG_SIZE] = "";
    ssize_t bytes_read = read(client_socket, client_msg, MAX_CLIENT_MSG_SIZE - 1);
    
    if (bytes_read < 0) {
        perror("Failed to read from client");
        close(client_socket);
        return;
    } else if (bytes_read == MAX_CLIENT_MSG_SIZE - 1) {
        // Read the remaining data to check if it exceeds the limit
        char extra;
        ssize_t extra_bytes = read(client_socket, &extra, 1);
        if (extra_bytes > 0) {
            printf("Request terlalu besar, menolak permintaan.\n");
            send(client_socket, "HTTP/1.1 413 Payload Too Large\r\n\r\n", 35, 0); // 413 status code
            close(client_socket);
            return; // Stop further processing
        }
    }
    client_msg[bytes_read] = '\0'; // Null-terminate the string

    // Parse HTTP method and route
    char method[8], urlRoute[256];
    sscanf(client_msg, "%s %s", method, urlRoute);
    printf("Method: %s\n Route: %s\n", method, urlRoute);

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
        char file_path[256] = "templates/";
        strcat(file_path, urlRoute); // Menambahkan nama file ke path

        // Cek jika file ada, lalu hapus
        if (deleteFile(file_path))
        {
            send_response(client_socket, "File deleted successfully.", 200);
        }
        else
        {
            send_response(client_socket, "File not found or cannot be deleted.", 404);
        }

        struct Route *deleted = deleteRoute(route, urlRoute);
        if (deleted) {
            send_response(client_socket, "Route deleted successfully.", 200);
        } else {
            send_response(client_socket, "Route not found.", 404);
        }
    } else if (strcmp(method, "PATCH") == 0) {
        // Handle PATCH request (update file content)
        char file_path[256] = "templates/";
        strcat(file_path, urlRoute); // Gabungkan dengan path dari URL

        // Cari body dari request HTTP
        char *body = strstr(client_msg, "\r\n\r\n");
        if (body)
        {
            body += 4; // Melewati terminator header

            // Buka file untuk ditulis ulang (mode append, jika ada isi akan ditimpa)
            FILE *file = fopen(file_path, "w");
            if (file)
            {
                fprintf(file, "%s", body); // Tulis konten baru ke file
                fclose(file);
                send_response(client_socket, "File updated successfully.", 200);
            }
            else
            {
                perror("Error opening file");
                send_response(client_socket, "File not found or cannot be updated.", 404);
            }
        }
        else
        {
            send_response(client_socket, "Invalid PATCH request body.", 400);
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
