#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "Routes.h"

void handle_client(int client_socket, struct Route *route);

#endif