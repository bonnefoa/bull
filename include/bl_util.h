#ifndef BL_UTIL_H
#define BL_UTIL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bullet.h>

void showInfoLog(GLuint object,
                PFNGLGETSHADERIVPROC glGet__iv,
                PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);

int max(int a, int b);
int min(int a, int b);
char *strduplicate(const char *s);

#endif

