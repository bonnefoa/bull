#include "bl_model.h"
#include <bl_log.h>
#include <bl_image.h>
#include <bl_matrix.h>
#include <bl_gl_util.h>

void BlModel::init()
{
        glGenBuffers(1, &indiceBuffer);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &normalBuffer);
        glGenBuffers(1, &tangentBuffer);
        glGenBuffers(1, &bitangentBuffer);

        uvBuffer = -1;
        if(blUVs.size() > 0){
                glGenBuffers(1, &uvBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
                std::vector<float> UV = blUVs[0].uvs;
                glBufferData(GL_ARRAY_BUFFER, UV.size() * sizeof(float)
                                , &UV[0], GL_STATIC_DRAW);
        }

        diffuseTextureBuffer = -1;
        normalTextureBuffer = -1;
        if(diffuseImage != NULL) {
                diffuseTextureBuffer = blTexture->fetchTexture(diffuseImage);
        }
        if(normalImage != NULL) {
                normalTextureBuffer = blTexture->fetchTexture(normalImage);
        }

        INFO("Generated buffers for %s: indice %i, vertex %i,\
normal %i, tangent %i, bitangent %i, uv %i, texture %i\n",
                        name, indiceBuffer, vertexBuffer, normalBuffer,
                        tangentBuffer, bitangentBuffer, uvBuffer, diffuseTextureBuffer);
        for(std::vector< std::vector<BlModel*> *>::iterator
                        it=children->begin();
                        it != children->end(); ++it) {
                for(std::vector< BlModel*>::iterator
                                it2=(*it)->begin();
                                it2 != (*it)->end(); ++it2) {
                        (*it2)->init();
                }
        }
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
        for(std::vector< std::vector<BlModel*> *>::iterator
                        it=children->begin();
                        it != children->end(); ++it) {
                for(std::vector< BlModel*>::iterator
                                it2=(*it)->begin();
                                it2 != (*it)->end(); ++it2) {
                        delete (*it2);
                }
        }
}

void BlModel::loadInBuffer()
{
        loadVectorsInBuffer(vertexBuffer, vertices);
        loadVectorsInBuffer(normalBuffer, normals);
        loadVectorsInBuffer(tangentBuffer, tangents);
        loadVectorsInBuffer(bitangentBuffer, bitangents);
        loadIndicesInBuffer(indiceBuffer, indices);

        for(std::vector< std::vector<BlModel*> *>::iterator
                        it=children->begin();
                        it != children->end(); ++it) {
                for(std::vector< BlModel*>::iterator
                                it2=(*it)->begin();
                                it2 != (*it)->end(); ++it2) {
                        (*it2)->loadInBuffer();
                }
        }
}

btTransform BlModel::getModelTansform()
{
        btTransform trans;
        trans.setIdentity();
        if(rigidBody && rigidBody->getMotionState()) {
                rigidBody->getMotionState()->getWorldTransform(trans);
        } else {
                trans.setOrigin(position);
        }
        return trans;
}

void BlModel::drawElement(GLint locModel, GLint locVertices,
                GLint locNormals, GLint locUVs,
                GLint locTangent, GLint locBitangent) {
        for(std::vector< std::vector<BlModel*> *>::iterator
                        it=children->begin();
                        it != children->end(); ++it) {
                for(std::vector< BlModel*>::iterator
                                it2=(*it)->begin();
                                it2 != (*it)->end(); ++it2) {
                        (*it2)->drawElement(locModel, locVertices,
                                        locNormals, locUVs,
                                        locTangent, locBitangent);
                }
        }

        if(locModel >= 0) sendTransform(getModelTansform(), locModel);
        bindVectors(locVertices, vertexBuffer);
        bindVectors(locNormals, normalBuffer);
        bindUVs(locUVs, uvBuffer);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTextureBuffer);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalTextureBuffer);

        drawIndices(indiceBuffer, indices.size());

        disableLocation(locNormals);
        disableLocation(locUVs);
        disableLocation(locVertices);
}
