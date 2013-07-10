#ifndef BL_MATRIX_H
#define BL_MATRIX_H

#include <bullet.h>
#include <GL/glew.h>
#include <GL/gl.h>

btTransform computeProjection(btScalar fov, btScalar aspect,
                btScalar zNear, btScalar zFar);

void sendTransform(btTransform trans, GLuint uniform);
void printBtTransform(btTransform &mat);
void printMatrix(btMatrix3x3 &mat);

btTransform computeOrthogonal(btScalar left, btScalar right,
                btScalar bottom, btScalar top,
                btScalar zNear, btScalar zFar);
btTransform computeViewMatrix(const btVector3 &right
                , const btVector3 &up
                , const btVector3 &direction
                , const btVector3 &position);
btTransform computeVPShadowMatrix(btVector3 position);
void bindProjectionMatrix(GLint programId, GLuint locProjection, btTransform projection);
btTransform buildMVPMatrix(btVector3 position, btTransform projection,
                btTransform view);
btTransform buildModelMatrix(btVector3 scale, btVector3 position);
btTransform translateBtTransform(btTransform source, btVector3 translation);

#endif
