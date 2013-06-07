#include "bl_model.h"
#include <bl_log.h>
#include <bl_image.h>
#include <bl_matrix.h>

void BlModel::init()
{
        glGenBuffers(1, &indiceBuffer);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &normalBuffer);
        glGenBuffers(1, &tangentBuffer);
        glGenBuffers(1, &bitangentBuffer);

        if(blUVs.size() > 0){
                glGenBuffers(1, &uvBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
                std::vector<float> UV = blUVs[0].uvs;
                glBufferData(GL_ARRAY_BUFFER, UV.size() * sizeof(float)
                                , &UV[0], GL_STATIC_DRAW);
        } else {
                uvBuffer = 0;
        }

        if(textureFile != NULL) {
                textureBuffer = blTexture->fetchTexture(textureFile);
        } else {
                textureBuffer = 0;
        }
        INFO("Generated buffers for %s: indice %i, vertex %i, normal %i, tangent %i, bitangent %i, uv %i, texture %i\n",
                        name, indiceBuffer, vertexBuffer, normalBuffer,
                        tangentBuffer, bitangentBuffer, uvBuffer, textureBuffer);
}

BlModel::~BlModel(void)
{
        glDeleteBuffers(1, &indiceBuffer);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &normalBuffer);
        glDeleteBuffers(1, &tangentBuffer);
        glDeleteBuffers(1, &bitangentBuffer);
        if(uvBuffer > 0)
                glDeleteBuffers(1, &uvBuffer);
}

void BlModel::loadInBuffer()
{
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                        vertices.size() * sizeof(btVector3),
                        &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER
                        , normals.size() * sizeof(btVector3)
                        , &normals[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
        glBufferData(GL_ARRAY_BUFFER
                        , tangents.size() * sizeof(btVector3)
                        , &tangents[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, bitangentBuffer);
        glBufferData(GL_ARRAY_BUFFER
                        , bitangents.size() * sizeof(btVector3)
                        , &bitangents[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER
                        , indices.size() * sizeof(unsigned int)
                        , &indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BlModel::bindVertices(GLint locVertices) {
        glEnableVertexAttribArray(locVertices);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(locVertices, 4 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);
}

void BlModel::bindNormals(GLint locNormals) {
        glEnableVertexAttribArray(locNormals);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glVertexAttribPointer(locNormals, 4 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);
}

void BlModel::bindUVs(GLint locUVs) {
        if(uvBuffer == 0) {
                return;
        }
        glEnableVertexAttribArray(locUVs);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(locUVs, 2, GL_FLOAT, GL_FALSE,
                        0, (void*)0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);
}

void BlModel::bindModelMatrix(GLint uniformM)
{
        btTransform trans;
        trans.setIdentity();
        if(rigidBody && rigidBody->getMotionState()) {
                rigidBody->getMotionState()->getWorldTransform(trans);
        } else {
                trans.setOrigin(position);
        }
        sendTransform(trans, uniformM);
}

void BlModel::drawElement() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void *)0);
}
