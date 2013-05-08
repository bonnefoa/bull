#include "bl_loader.h"
#include <bl_log.h>
#include <string.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdio.h>
#include <stdlib.h>

void getNodeFloat(xmlNode *node, const char *nodeName, float *res)
{
        char *end;
        if ((!xmlStrcmp(node->name, (const xmlChar*)nodeName))) {
                *res = strtof((const char *)node->children[0].content,
                                &end);
        }
}

btVector3 readPositionNode(xmlNode *node)
{
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        xmlNode *cur = node->xmlChildrenNode;
        while (cur != NULL) {
                getNodeFloat(cur, "x", &x);
                getNodeFloat(cur, "y", &y);
                getNodeFloat(cur, "z", &z);
                cur = cur->next;
        }
        return btVector3(x, y, z);
}

void readModelNode(xmlNode *node, btVector3 *position, float *mass)
{
        xmlNode *cur = node->xmlChildrenNode;
        while (cur != NULL) {
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"position"))) {
                        *position = readPositionNode(cur);
                }
                getNodeFloat(cur, "mass", mass);
                cur = cur->next;
        }
}

std::vector<BlModel*> loadAssetFile(const char *modelPath,
                btVector3 position, float mass)
{
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
                res.push_back(new BlModel(vertices, indices, position
                                        , mass, modelPath));
        }
        return res;

}

std::vector<BlModel*> loadModelNode(xmlNode *node)
{
        const char *modelPath = (const char *)xmlGetProp(node,
                        (const unsigned char *)"filename");
        btVector3 position = btVector3();
        float mass;
        readModelNode(node, &position, &mass);
        return loadAssetFile(modelPath, position, mass);
}

std::vector<BlModel*> *loadXmlScene(const char *filename)
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
