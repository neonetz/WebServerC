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
    }else
        {
            printf("%s\n", client_msg);
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