#include "bl_model.h"
#include <bl_log.h>
#include <bl_image.h>

void BlModel::init()
{
        glGenBuffers(1, &indiceBuffer);
        glGenBuffers(1, &vertexBuffer);

        if(blUVs.size() > 0){
                glGenBuffers(1, &uvBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
                std::vector<float> UV = blUVs[0].uvs;
                glBufferData(GL_ARRAY_BUFFER, UV.size() * sizeof(float)
                                , &UV[0], GL_STATIC_DRAW);
        } else {
                uvBuffer = 0;
        }

        if(image != NULL) {
                glGenTextures(1, &textureBuffer);
                BlImage *blImage = readPngImage(image);
                blImage->loadInBuffer(textureBuffer);
                delete blImage;
        } else {
                textureBuffer = 0;
        }
        INFO("Generated buffers : indice %i, vertex %i, uv %i, texture %i\n",
                        indiceBuffer, vertexBuffer, uvBuffer, textureBuffer);
}

void BlModel::clear(void)
{
        glDeleteBuffers(1, &indiceBuffer);
        glDeleteBuffers(1, &vertexBuffer);
        if(uvBuffer > 0)
                glDeleteBuffers(1, &uvBuffer);
        if(textureBuffer > 0)
                glDeleteTextures(1, &textureBuffer);
}

void BlModel::loadInBuffer()
{
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                        vertices.size() * sizeof(btVector3),
                        &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER
                        , indices.size() * sizeof(unsigned int)
                        , &indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void BlModel::drawElement(GLuint locationVertex, GLuint locationUv)
{
        glEnableVertexAttribArray(locationVertex);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(locationVertex, 4 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);

        if(uvBuffer > 0) {
                glEnableVertexAttribArray(locationUv);
                glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
                glVertexAttribPointer(locationUv, 2, GL_FLOAT, GL_FALSE,
                                0, (void*)0);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textureBuffer);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void *)0);

        glDisableVertexAttribArray(locationVertex);
        if(uvBuffer > 0) {
                glDisableVertexAttribArray(locationUv);
        }
}
