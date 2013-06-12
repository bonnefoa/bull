#include "bl_util.h"
#include <bl_log.h>
#include <stdio.h>
#include <stdlib.h>

int roundUpPowerOfTwo(int num)
{
        num--;
        num |= num >> 1;
        num |= num >> 2;
        num |= num >> 4;
        num |= num >> 8;
        num |= num >> 16;
        num++;
        return num;
}

void showInfoLog(GLuint object,
                PFNGLGETSHADERIVPROC glGet__iv,
                PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
        GLint log_length;
        char *log;
        glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
        log = (char *) malloc(log_length);
        glGet__InfoLog(object, log_length, NULL, log);
        INFO("Message : %s\n", log);
        free(log);
}

int max(int a, int b)
{
        if (a > b)
                return a;
        return b;
}

int min(int a, int b)
{
        if (a < b)
                return a;
        return b;
}

char *strduplicate(const char *s)
{
        char *res;
        int numChars = strlen(s) + 1;
        if((res = (char *)malloc(sizeof(char) * numChars)) == NULL)
                return NULL;
        strncpy(res,s, numChars);
        return res;
}

void setIfMax(btScalar point, btScalar ref,
                btVector3 &p, void (btVector3::*f)(btScalar))
{
        point = abs(point);
        if (ref < point) (p.*f)(point);
}

float **initMatrix(int width, int height)
{
        float **mat = (float **)malloc(width * sizeof(float*));
        for(int x = 0; x < width; x++) {
                mat[x] = (float *)malloc(height * sizeof(float));
                memset(mat[x], 0, height * sizeof(float));
        }
        return mat;
}

void freeMatrix(float **mat, int width)
{
        for(int x = 0; x < width; x++) {
                free(mat[x]);
        }
        free(mat);
}

float ***initMatrixVector(int width, int height, int sizeVector)
{
        float ***mat = (float ***)malloc(width * sizeof(float**));
        for(int x = 0; x < width; x++) {
                mat[x] = (float **)malloc(height * sizeof(float*));
                for(int y = 0; y < height; y++) {
                        mat[x][y] = (float *)malloc(
                                        sizeVector * sizeof(float));
                        memset(mat[x][y], 0, sizeVector * sizeof(float));
                }
        }
        return mat;
}

void freeMatrixVector(float ***mat, int width, int height)
{
        for(int x = 0; x < width; x++) {
                for(int y = 0; y < height; y++) {
                        free(mat[x][y]);
                }
                free(mat[x]);
        }
        free(mat);
}

std::vector <btVector3> matrixVectorToVectorList(float ***arr, int width, int lenght)
{
        std::vector <btVector3> vect;
        for(int z = 0; z < lenght; z++) {
                for(int x = 0; x < width; x++) {
                        btVector3 vert = btVector3(arr[x][z][0],
                                        arr[x][z][1],
                                        arr[x][z][2]);
                        vect.push_back(vert);
                }
        }
        return vect;
}
