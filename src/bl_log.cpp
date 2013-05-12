#include "bl_log.h"
#include <stdarg.h>

void dieMessage(const char* tag, const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        logMessage(stderr, tag, fmt, ap);
        va_end(ap);
        exit(1);
}

void logMessage(FILE *stream, const char* tag, const char* fmt, ...)
{
        fprintf(stream, "[%s]: ", tag);
        va_list ap;
        va_start(ap, fmt);
        vfprintf(stream, fmt, ap);
        va_end(ap);
}
