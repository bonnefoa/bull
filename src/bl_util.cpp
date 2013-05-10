#include "bl_util.h"
#include <bl_log.h>
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
        ERROR("%s", log);
        free(log);
}

void printBtTransform(btTransform *mat)
{
       btMatrix3x3 bas = mat->getBasis();
       btVector3 ori = mat->getOrigin();
       for (int n = 0; n < 3; n++) {
                for (int m = 0; m < 3; m++) {
                        printf("%f ", bas[n][m]);
                }
                printf("%f ", ori[n]);
                printf("\n");
       }
       printf("%f %f %f %f\n\n", 0.f, 0.f, 0.f, 1.f);
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
