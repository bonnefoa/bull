#ifndef BL_GL_UTIL_H
#define BL_GL_UTIL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bullet.h>
#include <vector>

void bindBufferToLocation(GLint location, GLuint buffer, GLint size);
void disableLocation(GLint location);
void loadVectorsInBuffer(GLuint buffer, std::vector<btVector3> &vectors);

void loadUVsInBuffer(GLuint buffer, std::vector<float> &uvs);
void loadIndicesInBuffer(GLuint buffer, std::vector<unsigned int> &indices);
void drawIndices(GLuint indiceBuffer, int size);


#endif
