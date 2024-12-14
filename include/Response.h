#ifndef RESPONSE_H
#define RESPONSE_H	
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HTTP_Server.h"
#include "Routes.h"
#define MAX_REQUEST_SIZE 4096

char * render_static_file(char* fileName);
