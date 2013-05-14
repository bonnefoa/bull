#include "bl_util.h"
#include <bl_log.h>
#include <stdio.h>
#include <stdlib.h>

void showInfoLog(GLuint object,
                PFNGLGETSHADERIVPROC glGet__iv,
                PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
        GLint log_length;
        char *log;
        glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
        log = (char *) malloc(log_length);
        glGet__InfoLog(object, log_length, NULL, log);
        INFO("Message : %s\n", log);
        free(log);
}

int max(int a, int b)
{
        if (a > b) return a;
        return b;
}

int min(int a, int b)
{
        if (a < b) return a;
        return b;
}

char *strduplicate(const char *s)
{
        char *res;
        int numChars = strlen(s) + 1;
        if((res = (char *)malloc(sizeof(char) * numChars)) == NULL)
                return NULL;
        strncpy(res,s, numChars);
        return res;
}
