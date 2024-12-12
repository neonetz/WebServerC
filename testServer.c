#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

void handle_client(int client_socket) {
    // Buffer untuk membaca data
    char buffer[1024] = {0};
    read(client_socket, buffer, sizeof(buffer));
    printf("Request:\n%s\n", buffer);

    // Respons HTTP sederhana
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 55\r\n"
        "\r\n"
        "<html><body><h1>Welcome to C Web Server!</h1></body></html>";

    // Kirim respons ke klien
    write(client_socket, response, strlen(response));

    // Tutup koneksi
    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Membuat socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind ke port dan alamat
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen untuk koneksi masuk
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Web server running on port %d...\n", PORT);

    // Loop menerima koneksi
    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Tangani permintaan klien
        handle_client(client_socket);
    }

    return 0;
}
