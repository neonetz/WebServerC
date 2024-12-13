#include "Logger.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

void log_request(const char *method, const char *url)
{
    FILE *log_file = fopen("server.log", "a");
    if (log_file != NULL)
    {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[26];
        strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info); // Format timestamp

        fprintf(log_file, "[%s] Method: %s, URL: %s\n", time_str, method, url);
        fclose(log_file);
    }
    else
    {
        perror("Failed to open log file");
    }
}