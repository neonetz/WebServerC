#include "Response.h"

// Fungsi untuk menangani koneksi klien
void process_client_connection(int server_socket, struct Route *route, HTTP_Server *http_server) {
    int client_socket;
    while (1) {
        client_socket = accept(server_socket, NULL, NULL);

        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        log_activity("New client connected.");

        sem_wait(&http_server->semaphore);

        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            close(client_socket);
            sem_post(&http_server->semaphore);
            continue;
        }

        if (pid == 0) {
            close(server_socket);
            handle_client(client_socket, route, http_server);
            sem_post(&http_server->semaphore);
            exit(0);
        } else {
            close(client_socket);
        }
    }
}




// Fungsi untuk menangani klien
void handle_client(int client_socket, struct Route *route, HTTP_Server *http_server) {
    char client_msg[MAX_REQUEST_] = "";
    if (read(client_socket, client_msg, sizeof(client_msg)-1) < 0) {
        perror("Failed to read data");
        close(client_socket);
        sem_post(&http_server->semaphore);
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

    printf("Method: %s\n", method);
    printf("Route: %s\n", urlRoute);

    char template[100] = "";

    if (!validate_url(urlRoute)) {
        strcpy(template, "templates/404.html"); // Bad request
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
    char http_header[MAX_REQUEST_] = "HTTP/1.1 200 OK\r\n\r\n";
    strcat(http_header, response_data);
    strcat(http_header, "\r\n\r\n");

    // Mengirim respons ke klien
    send(client_socket, http_header, sizeof(http_header), 0);
    log_activity("Client request handled.");
    close(client_socket);
    free(response_data);
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

char * render_static_file(char * fileName) {
	FILE* file = fopen(fileName, "r");

	if (file == NULL) {
		return NULL;
	}else {
		printf("%s does exist \n", fileName);
	}

	fseek(file, 0, SEEK_END);
	long fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* temp = malloc(sizeof(char) * (fsize+1));
	char ch;
	int i = 0;
	while((ch = fgetc(file)) != EOF) {
		temp[i] = ch;
		i++;
	}
	fclose(file);
	return temp;
}
