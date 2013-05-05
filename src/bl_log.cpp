#include "bl_log.h"

void logMessage(FILE *stream, const char* tag, const char* fmt, ...)
{
        time_t now;
        time(&now);
        fprintf(stream, "%s [%s]: ", ctime(&now), tag);

        va_list ap;
        va_start(ap, fmt);
        vfprintf(stdout, fmt, ap);
        va_end(ap);
}
