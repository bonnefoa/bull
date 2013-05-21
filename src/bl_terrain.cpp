#include "bl_terrain.h"
#include <bl_image.h>
#include <bl_matrix.h>
#include <bl_log.h>

BlTerrain::BlTerrain(unsigned int _verticeNumber,
                          btTransform _model,
                          const char *_image)
        : model(_model), image(_image), verticeNumber(_verticeNumber)
{
        for(unsigned int z = 0; z < verticeNumber; z++) {
                for(unsigned int x = 0; x < verticeNumber; x++) {
                        btVector3 vert = btVector3(x, 0, z);
                        vertices.push_back(vert);
                }
        }
        unsigned int maxZ = verticeNumber * (verticeNumber - 1);
        for(unsigned int z = 0; z < maxZ; z+=verticeNumber) {
                for(unsigned int x = z; x < z + verticeNumber - 1; x++) {
                        indices.push_back(x);
                        indices.push_back(x + verticeNumber);
                        indices.push_back(x + 1);

                        indices.push_back(x + 1);
                        indices.push_back(x + verticeNumber);
                        indices.push_back(x + verticeNumber + 1);
                }
        }
}

void BlTerrain::init()
{
        glGenBuffers(1, &indiceBuffer);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &normalBuffer);

        if(image != NULL) {
                INFO("Loading image %s for terrain\n", image);
                glGenTextures(1, &textureBuffer);
                BlImage *blImage = readPngImage(image);
                blImage->loadInBuffer(textureBuffer);
                delete blImage;
        } else {
                textureBuffer = 0;
        }
}

BlTerrain::~BlTerrain()
{
        glDeleteBuffers(1, &indiceBuffer);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &normalBuffer);
        if(textureBuffer > 0)
                glDeleteTextures(1, &textureBuffer);
}

void BlTerrain::loadInBuffer()
{
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                        vertices.size() * sizeof(btVector3),
                        &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER
                        , normals.size() * sizeof(btVector3)
                        , &normals[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER
                        , indices.size() * sizeof(unsigned int)
                        , &indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);
}

void BlTerrain::bindVertices(GLint locVertices)
{
        glEnableVertexAttribArray(locVertices);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(locVertices, 4 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);
}

void BlTerrain::bindVerticeNumber(GLint locSizeTerrain)
{
        glUniform1i(locSizeTerrain, verticeNumber);
}

void BlTerrain::bindTextures()
{
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);
}

void BlTerrain::bindModelMatrix(GLint uniformModel)
{
        sendTransform(model, uniformModel);
}

void BlTerrain::drawElement() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void *)0);
}
