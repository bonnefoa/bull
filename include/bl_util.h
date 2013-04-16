#ifndef BL_UTIL_H
#define BL_UTIL_H

#include <GL/glew.h>
#include <GL/gl.h>

void show_info_log(GLuint object,
                PFNGLGETSHADERIVPROC glGet__iv,
                PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);

#endif

