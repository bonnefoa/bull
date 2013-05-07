#include "bl_loader.h"
#include <bl_log.h>
#include <string.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdio.h>
#include <stdlib.h>


btVector3 readPositionNode(xmlNode *node)
{
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        char *end;
        xmlNode *cur = node->xmlChildrenNode;
        while (cur != NULL) {
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"x"))) {
                        x = strtof((const char *)cur->children[0].content,
                                        &end);
                }
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"y"))) {
                        y = strtof((const char *)cur->children[0].content,
                                        &end);
                }
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"z"))) {
                        z = strtof((const char *)cur->children[0].content,
                                        &end);
                }
                cur = cur->next;
        }
        return btVector3(x, y, z);
}

btVector3 readPosition(xmlNode *node)
{
        xmlNode *cur = node->xmlChildrenNode;
        while (cur != NULL) {
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"position"))) {
                        return readPositionNode(cur);
                }
                cur = cur->next;
        }
        return btVector3(0,0,0);
}

std::vector<BlModel*> loadModelNode(xmlNode *node)
{
        const char *modelPath = (const char *)xmlGetProp(node,
                        (const unsigned char *)"filename");
        btVector3 position = readPosition(node);
        INFO("Loading asset from file %s\n", modelPath);
        std::vector<BlModel*> res = std::vector<BlModel*>();

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
                return res;
        }

        for (unsigned int i = 0; i < scene->mNumMeshes; i++){
                std::vector <btVector3> vertices;
                std::vector <unsigned int> indices;
                aiMesh * mesh = scene->mMeshes[i];
                const char *meshName = mesh->mName.C_Str();
                INFO("Process mesh %i %s\n", i, meshName);
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
                res.push_back(new BlModel(vertices, indices, position));
        }
        return res;
}

std::vector<BlModel*> *loadScene(char *filename)
{
        xmlDoc         *document;
        xmlNode        *root, *node;

        std::vector<BlModel*> *models = new std::vector<BlModel*>();

        document = xmlReadFile(filename, NULL, 0);
        root = xmlDocGetRootElement(document);
        for (node = root->children; node; node = node->next) {
                if(strcmp((char *)node->name, "model") == 0) {
                        std::vector<BlModel*> nodeModels = loadModelNode(node);
                        for (unsigned int i = 0; i < nodeModels.size(); i++) {
                                models->push_back(nodeModels[i]);
                        }
                }
        }
        return models;
}
