#include "bl_loader.h"
#include <bl_log.h>
#include <bl_image.h>
#include <bl_util.h>
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

void readModelNode(xmlNode *node, btVector3 *position, float *mass, char **image)
{
        xmlNode *cur = node->xmlChildrenNode;
        while (cur != NULL) {
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"position"))) {
                        *position = readPositionNode(cur);
                }
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"image"))) {
                        const char *attr = (const char *)xmlGetProp(cur,
                                        (const unsigned char *)"path");
                        *image = strduplicate(attr);
                }
                getNodeFloat(cur, "mass", mass);
                cur = cur->next;
        }
}

std::vector <btVector3> loadVertices(aiMesh *mesh)
{
        std::vector <btVector3> vertices;
        for(unsigned int j=0; j < mesh->mNumVertices; j++){
                aiVector3D meshVert = mesh->mVertices[j];
                btVector3 vertice = btVector3(-meshVert.y,
                                meshVert.z, -meshVert.x);
                vertices.push_back(vertice);
        }
        return vertices;
}

std::vector <unsigned int> loadIndices(aiMesh *mesh)
{
        std::vector <unsigned int > indices;
        for(unsigned int j=0; j < mesh->mNumFaces; j++){
                aiFace meshFace = mesh->mFaces[j];
                for(unsigned int k=0; k < meshFace.mNumIndices; k+=3) {
                        indices.push_back(meshFace.mIndices[k]);
                        indices.push_back(meshFace.mIndices[k+1]);
                        indices.push_back(meshFace.mIndices[k+2]);
                }
        }
        return indices;
}

std::vector< BlUvs > loadUvs(aiMesh *mesh)
{
        int textureIndice = 0;
        std::vector< BlUvs > uvs;

        while(textureIndice < AI_MAX_NUMBER_OF_TEXTURECOORDS) {
                if (!mesh->HasTextureCoords(textureIndice)){
                        break;
                }
                unsigned int numComponent = mesh->mNumUVComponents[textureIndice];
                std::vector <float> currentUvs;
                aiVector3D *tex = mesh->mTextureCoords[textureIndice];
                for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
                        aiVector3D currentVector = tex[i];
                        for(unsigned j = 0; j < numComponent; j++) {
                                currentUvs.push_back(currentVector[j]);
                        }
                }
                uvs.push_back(BlUvs(currentUvs, numComponent));
                textureIndice++;
        }
        return uvs;
}

std::vector<BlModel*> loadAssetFile(const char *modelPath,
                btVector3 position, float mass,
                char *image)
{
        INFO("Loading asset from file %s, mass %f, image %s\n", modelPath
                        , mass, image);
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
                aiMesh * mesh = scene->mMeshes[i];
                INFO("Process mesh %i\n", i);

                std::vector <btVector3> vertices = loadVertices(mesh);
                std::vector <unsigned int> indices = loadIndices(mesh);
                std::vector <BlUvs> uvs = loadUvs(mesh);

                BlModel *blModel = new BlModel(vertices, indices, uvs,
                                        position, mass, modelPath, image);
                res.push_back(blModel);
        }
        return res;

}

std::vector<BlModel*> loadModelNode(xmlNode *node)
{
        const char *modelPath = (const char *)xmlGetProp(node,
                        (const unsigned char *)"filename");
        btVector3 position = btVector3();
        float mass;
        char *image = NULL;
        readModelNode(node, &position, &mass, &image);
        return loadAssetFile(modelPath, position, mass, image);
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
