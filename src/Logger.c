#include "Logger.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

/**
 * @brief Logs an HTTP request to a log file.
 *
 * This function logs the HTTP request method and URL to a file named "server.log".
 * Each log entry is timestamped with the current date and time.
 *
 * @param method The HTTP request method (e.g., "GET", "POST").
 * @param url The URL of the HTTP request.
 */
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

/**
 * @brief Logs a fork activity message with a timestamp and process ID to a log file.
 *
 * This function opens the "serverFork.log" file in append mode and writes a log entry
 * containing the current timestamp, a custom message, and the process ID (PID). If the
 * log file cannot be opened, an error message is printed to standard error.
 *
 * @param message The custom message to log.
 * @param pid The process ID to log.
 */
void log_fork(const char * message, int pid){
    FILE *log_file = fopen("serverFork.log", "a");
    if (log_file != NULL)
    {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[26];
        strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info); // Format timestamp

        fprintf(log_file, "[%s] Fork Activity : %s, PID: %d\n", time_str, message, pid);
        fclose(log_file);
    }
    else
    {
        perror("Failed to open log file");
    }
}