#ifndef BL_MATRIX_H
#define BL_MATRIX_H

#include <bullet.h>
#include <GL/glew.h>
#include <GL/gl.h>

btTransform computeProjection(btScalar fov, btScalar aspect,
                btScalar zNear, btScalar zFar);

void sendTransform(btTransform trans, GLuint uniform);
void printBtTransform(btTransform *mat);

#endif
