#ifndef BL_GL_UTIL_H
#define BL_GL_UTIL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bullet.h>
#include <vector>

void bindVectors(GLint location, GLuint buffer);
void loadVectorsInBuffer(GLuint buffer, std::vector<btVector3> &vectors);


#endif
