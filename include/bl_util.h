#ifndef BL_UTIL_H
#define BL_UTIL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bullet.h>
#include <vector>

#define print_vector(prefix, vect) \
        INFO("%s %f %f %f\n", prefix, vect[0], vect[1], vect[2]);

void showInfoLog(GLuint object,
                PFNGLGETSHADERIVPROC glGet__iv,
                PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);

int max(int a, int b);
int min(int a, int b);
char *strduplicate(const char *s);
void setIfMax(btScalar point, btScalar ref,
                btVector3 &p, void (btVector3::*f)(btScalar));

float **initMatrix(int width, int height);
float ***initMatrixVector(int width, int height, int sizeVector);
std::vector <btVector3> matrixVectorToVectorList(float ***arr, int width, int lenght);
void freeMatrixVector(float ***mat, int width, int height);
void freeMatrix(float **mat, int width);
int roundUpPowerOfTwo(int num);

#endif
