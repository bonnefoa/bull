#include "bl_util.h"
#include <stdio.h>
#include <stdlib.h>

void show_info_log(GLuint object,
                PFNGLGETSHADERIVPROC glGet__iv,
                PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
        GLint log_length;
        char *log;
        glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
        log = (char *) malloc(log_length);
        glGet__InfoLog(object, log_length, NULL, log);
        fprintf(stderr, "%s", log);
        free(log);
}
