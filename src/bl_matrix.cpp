#include "bl_matrix.h"
#include <math.h>
#include <bl_log.h>

void sendTransform(btTransform trans, GLuint uniform)
{
        btScalar mat[16];
        trans.getOpenGLMatrix(mat);
        glUniformMatrix4fv(uniform, 1, GL_FALSE, mat);
}

void printBtTransform(btTransform &mat)
{
       btMatrix3x3 bas = mat.getBasis();
       btVector3 ori = mat.getOrigin();
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

btTransform computeOrthogonal(btScalar left, btScalar right,
                btScalar bottom, btScalar top,
                btScalar zNear, btScalar zFar)
{
        btMatrix3x3 basis(
                        2.0f / (right - left), 0, 0
                        , 0, 2.0f / (top - bottom), 0
                        , 0, 0, -2.0f / (zFar - zNear)
                        );
        btScalar xTrans = - (right + left) / (right - left);
        btScalar yTrans = - (top + bottom) / (top - bottom);
        btScalar zTrans = - (zFar + zNear) / (zFar - zNear);
        btVector3 origin = btVector3(xTrans, yTrans, zTrans);
        return btTransform(basis, origin);
}

btTransform computeViewMatrix(const btVector3 &right
                , const btVector3 &up
                , const btVector3 &direction
                , const btVector3 &position)
{
        btTransform transform;
        btMatrix3x3 basis;
        basis[0] = right;
        basis[1] = up;
        basis[2] = -1.0f * direction;
        basis = basis.transpose();
        btTransform view = btTransform(basis, position);
        view = view.inverse();
        return view;
}

btTransform computeVPShadowMatrix(btVector3 position)
{
        btTransform depthProjectionMatrix = computeOrthogonal(-10, 10, -10,
                        10, -10, 10);
        position = position.normalize();
        float phi = atan(position.getX() / position.getZ());

        btVector3 direction = -btVector3(position);
        btVector3 right = btVector3(sin(phi + M_PI_2), 0 , cos(phi + M_PI_2));
        btVector3 up = right.cross(direction);
        btTransform depthViewMatrix = computeViewMatrix(right, up,
                        direction, position);
        return depthProjectionMatrix * depthViewMatrix;
}


void bindProjectionMatrix(GLint programId, GLuint locProjection, btTransform projection)
{
        glUseProgram(programId);

        btScalar mat[16];
        projection.getOpenGLMatrix(mat);
        mat[11] = -1.0f;
        glUniformMatrix4fv(locProjection, 1, GL_FALSE, mat);
}

btTransform buildMVPMatrix(btVector3 position, btTransform projection,
                btTransform view)
{
        btTransform model;
        model.setIdentity();
        model.setOrigin(position);

        btTransform MVP = projection * view * model;
        return MVP;
}

btTransform buildModelMatrix(btVector3 scale, btVector3 position)
{
        btMatrix3x3 basis;
        basis.setIdentity();
        basis = basis.scaled(scale);
        btTransform result;
        result.setBasis(basis);
        result.setOrigin(position);
        return result;
}
