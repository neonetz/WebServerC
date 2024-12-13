#ifndef LOGGER_H
#define LOGGER_H

void log_request(const char *method, const char *url);
void log_fork(const char *message, int pid);

#endif