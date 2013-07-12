#include "bl_skybox.h"
#include <bl_matrix.h>
#include <bl_gl_util.h>
#include <bl_log.h>

const GLfloat BlSkybox::cubeVertices[24] = {
        -1.0,  1.0,  1.0,
        -1.0, -1.0,  1.0,
        1.0, -1.0,  1.0,
        1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0,
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,
        1.0,  1.0, -1.0,
};

const GLushort BlSkybox::cubeIndices[24] = {
        3, 2, 1, 0,
        7, 6, 2, 3,
        4, 5, 6, 7,
        0, 1, 5, 4,
        4, 7, 3, 0,
        5, 6, 2, 1,
};

const GLenum BlSkybox::targetTypes[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

BlSkybox::BlSkybox( const char *pathXpos,
                const char *pathXneg,
                const char *pathYpos,
                const char *pathYneg,
                const char *pathZpos,
                const char *pathZneg)
{
        imagePaths[0] = pathXpos;
        imagePaths[1] = pathXneg;
        imagePaths[2] = pathYpos;
        imagePaths[3] = pathYneg;
        imagePaths[4] = pathZpos;
        imagePaths[5] = pathZneg;
}

void BlSkybox::init()
{
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &indiceBuffer);

        glActiveTexture(GL_TEXTURE10);
        glEnable(GL_TEXTURE_CUBE_MAP);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glGenTextures(1, &cubeMapTexture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        for (int i = 0; i < 6; i++) {
                BlImage *image = new BlImage(imagePaths[i]);
                image->loadInCubeMap(targetTypes[i]);
                delete image;
        }
}

void BlSkybox::loadInBuffer()
{
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                        sizeof(cubeVertices),
                        cubeVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        sizeof(cubeIndices),
                        &cubeIndices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BlSkybox::drawSkybox(GLint locModel,
                GLint locVertices)
{
        btTransform model = buildModelMatrix(btVector3(200, 200, 200), btVector3(0,0,0));
        sendTransform(model, locModel);

        glActiveTexture(GL_TEXTURE10);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

        glEnableVertexAttribArray(locVertices);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(locVertices, 3 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, 0);

        glDisableVertexAttribArray(locVertices);
}

BlSkybox::~BlSkybox()
{
        glDeleteTextures(1, &cubeMapTexture);
        glDeleteBuffers(1, &vertexBuffer);
}
