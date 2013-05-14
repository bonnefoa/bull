#include "bl_matrix.h"

void sendTransform(btTransform trans, GLuint uniform)
{
        btScalar mat[16];
        trans.getOpenGLMatrix(mat);
        glUniformMatrix4fv(uniform, 1, GL_FALSE, mat);
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

