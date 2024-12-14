#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "Routes.h"

void handle_client(int client_socket, struct Route *route);
int deleteFile(const char *filePath);

    void send_response(int client_socket, const char *body, int status_code);
#endif