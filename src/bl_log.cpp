#include "bl_log.h"

void logMessage(FILE *stream, const char* tag, const char* fmt, ...)
{
        fprintf(stream, "[%s]: ", tag);

        va_list ap;
        va_start(ap, fmt);
        vfprintf(stdout, fmt, ap);
        va_end(ap);
}
