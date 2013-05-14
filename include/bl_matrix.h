#ifndef BL_MATRIX_H
#define BL_MATRIX_H

#include <bullet.h>

btTransform computeProjection(btScalar fov, btScalar aspect,
                btScalar zNear, btScalar zFar);

#endif
