
#include "HTTP_Server.h"
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>

/**
 * @brief Initializes the HTTP server.
 *
 * This function sets up the HTTP server by creating a socket, binding it to the specified port,
 * and setting it to listen for incoming connections.
 *
 * @param http_server A pointer to the HTTP_Server structure to initialize.
 * @param port The port number on which the server will listen for incoming connections.
 */
void init_server(HTTP_Server * http_server, int port) {
	http_server->port = port;

	int server_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = INADDR_ANY;

	bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

	listen(server_socket, 5);

	http_server->socket = server_socket;
	printf("HTTP Server Initialized\nPort: %d\n", http_server->port);
}
