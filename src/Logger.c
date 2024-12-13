#include "Logger.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

void log_request(const char *method, const char *url) {
    FILE *log_file = fopen("server.log", "a");
    if (log_file != NULL) {
        time_t now = time(NULL);
        char *time_str = ctime(&now);
        time_str[strlen(time_str) - 1] = '\0'; // Menghapus newline di akhir

        fprintf(log_file, "[%s] Method: %s, URL: %s\n", time_str, method, url);
        fclose(log_file);
    } else {
        perror("Failed to open log file");
    }
}