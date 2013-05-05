#ifndef BL_LOG_H
#define BL_LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INFO(message, args...) logMessage(stdout, __FILE__, message, ##args)
#define ERROR(message, args...) logMessage(stderr, __FILE__, message, ##args)

void logMessage(FILE *stream, const char* tag, const char* fmt, ...);

#endif
