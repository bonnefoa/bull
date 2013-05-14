#include "bl_matrix.h"

btTransform computeProjection(btScalar fov, btScalar aspect,
                btScalar zNear, btScalar zFar)
{
        btScalar f = 1.f / tan(fov / 2.f);
        btScalar xRot = f / aspect;
        btScalar yRot = f;
        btScalar zRot = (zFar + zNear) / (zNear - zFar);
        btScalar zTrans = (2.0f * zFar * zNear) / (zNear - zFar);
        btMatrix3x3 basis(xRot, 0, 0
                        , 0, yRot, 0
                        , 0, 0, zRot);
        btVector3 origin = btVector3(0, 0, zTrans);
        return btTransform(basis, origin);
}

