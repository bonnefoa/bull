#include "bl_model.h"
#include <bl_log.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void BlModel::init()
{
        glGenBuffers(1, &indiceBuffer);
        glGenBuffers(1, &vertexBuffer);
}

void BlModel::loadInBuffer()
{
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(btVector3)
                        , &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER
                        , indices.size() * sizeof(unsigned int)
                        , &indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void BlModel::drawElement(GLuint locationVertex)
{
        glEnableVertexAttribArray(locationVertex);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(locationVertex, 4 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)0);

        glDisableVertexAttribArray(locationVertex);
}

bool BlModel::loadAsset(void)
{
        Assimp::Importer importer;
        const aiScene * scene = importer.ReadFile(modelPath,
                        aiProcess_CalcTangentSpace
                        | aiProcess_JoinIdenticalVertices
                        | aiProcess_Triangulate
                        | aiProcess_OptimizeMeshes
                        | aiProcess_SortByPType
                        | aiProcess_FindInstances
                        );
        if (!scene) {
                ERROR("Could not load scene. %s\n"
                                , importer.GetErrorString());
                return false;
        }

        for (unsigned int i = 0; i < scene->mNumMeshes; i++){
                aiMesh * mesh = scene->mMeshes[i];
                INFO("Process mesh %i\n", i);
                for(unsigned int j=0; j < mesh->mNumVertices; j++){
                        aiVector3D meshVert = mesh->mVertices[j];
                        btVector3 vertice = btVector3(meshVert.x
                                        , meshVert.y, meshVert.z);
                        vertices.push_back(vertice);
                }

                for(unsigned int j=0; j < mesh->mNumFaces; j++){
                        aiFace meshFace = mesh->mFaces[j];
                        for(unsigned int k=0; k < meshFace.mNumIndices; k+=3) {
                                indices.push_back(meshFace.mIndices[k]);
                                indices.push_back(meshFace.mIndices[k+1]);
                                indices.push_back(meshFace.mIndices[k+2]);
                        }
                }

        }
        return true;
}
