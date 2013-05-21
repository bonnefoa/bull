#include "bl_terrain.h"
#include <bl_image.h>

BlTerrain::BlTerrain(unsigned int vertNumbers,
                          const char *_image)
        : image(_image)
{
        for(unsigned int z = 0; z < vertNumbers; z++) {
                for(unsigned int x = 0; x < vertNumbers; x++) {
                        btVector3 vert = btVector3(x, 0, z);
                        vertices.push_back(vert);
                }
        }
        for(unsigned int z = 0; z < vertices.size(); z+=vertNumbers) {
                for(unsigned int x = z; x < z + vertNumbers; x++) {
                        indices.push_back(x);
                        indices.push_back(x + vertNumbers);
                        indices.push_back(x + 1);

                        indices.push_back(x + 1);
                        indices.push_back(x + vertNumbers);
                        indices.push_back(x + vertNumbers + 1);
                }
        }
}

void BlTerrain::init()
{
        glGenBuffers(1, &indiceBuffer);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &normalBuffer);

        if(image != NULL) {
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

void BlTerrain::drawElement() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void *)0);
}
