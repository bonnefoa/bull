#include "bl_loader.h"
#include <bl_log.h>
#include <bl_image.h>
#include <bl_light_point.h>
#include <bl_util.h>
#include <string.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdio.h>
#include <stdlib.h>

btVector3 convertAiColorToBtVector(aiColor3D color)
{
        return btVector3(color[0], color[1], color[2]);
}

btVector3 convertAiVectorToBtVector(aiVector3D vec)
{
        return btVector3(-vec.y, vec.z, -vec.x);
}

Assimp::Importer importer;

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

std::vector <btVector3> loadVertices(aiMesh *mesh)
{
        std::vector <btVector3> vertices;
        for(unsigned int j=0; j < mesh->mNumVertices; j++){
                vertices.push_back(convertAiVectorToBtVector(mesh->mVertices[j]));
        }
        return vertices;
}

std::vector <btVector3> loadVerticesInformation(aiMesh *mesh,
                aiVector3D *infos)
{
        std::vector <btVector3> result;
        if(infos == NULL) return result;
        for(unsigned int j=0; j < mesh->mNumVertices; j++){
                result.push_back(convertAiVectorToBtVector(infos[j]));
        }
        return result;
}

std::vector <btVector3> loadNormals(aiMesh *mesh)
{
        std::vector <btVector3> normals;
        for(unsigned int j=0; j < mesh->mNumVertices; j++){
                normals.push_back(convertAiVectorToBtVector(mesh->mNormals[j]));
        }
        return normals;
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

const aiScene *loadScene(const char *path)
{
        const aiScene *scene = importer.ReadFile(path,
                        aiProcess_CalcTangentSpace
                        | aiProcess_Triangulate
                        );
        if (!scene) {
                ERROR("Could not load scene. %s\n"
                                , importer.GetErrorString());
                return NULL;
        }
        return scene;
}

std::vector<BlModel*> loadModelFile(const char *modelPath,
                btVector3 position, float mass, char *image)
{
        INFO("Loading asset from file %s, mass %f, image %s, position %f %f %f\n",
                        modelPath, mass, image,
                        position[0], position[1], position[2]);
        std::vector<BlModel*> res = std::vector<BlModel*>();
        const aiScene *scene = loadScene(modelPath);

        for (unsigned int i = 0; i < scene->mNumMeshes; i++){
                aiMesh * mesh = scene->mMeshes[i];
                std::vector <btVector3> vertices = loadVertices(mesh);
                std::vector <btVector3> normals = loadVerticesInformation(
                                mesh, mesh->mNormals);

                std::vector <btVector3> tangents = loadVerticesInformation(
                                mesh, mesh->mTangents);
                std::vector <btVector3> bitangents = loadVerticesInformation(
                                mesh, mesh->mBitangents);
                std::vector <unsigned int> indices = loadIndices(mesh);
                std::vector <BlUvs> uvs = loadUvs(mesh);
                INFO("Got %i vertices, %i indices, %i uvs, %i normals, %i tangents, %i bitangents\n",
                                vertices.size(), indices.size(), uvs.size(),
                                normals.size(), tangents.size(),
                                bitangents.size());
                BlModel *blModel = new BlModel(vertices, indices, normals,
                                tangents, bitangents,
                                uvs, position, mass, modelPath, image);
                res.push_back(blModel);
        }
        return res;
}

BlLightAmbient *loadAmbientFile(const char *path)
{
        INFO("Loading ambient from file %s\n", path);
        const aiScene *scene = loadScene(path);
        if (scene->mNumLights > 0 ) {
                aiLight * light = scene->mLights[0];
                btVector3 color = convertAiColorToBtVector(light->mColorDiffuse);
                return new BlLightAmbient(color);
        }
        return new BlLightAmbient(btVector3());
}

std::vector<BlLightPoint*> loadLightFile(const char *path, btVector3 position)
{
        INFO("Loading light from file %s, position %f %f %f\n",
                        path, position[0], position[1], position[2]);
        std::vector<BlLightPoint*> res = std::vector<BlLightPoint*>();
        const aiScene *scene = loadScene(path);
        for (unsigned int i = 0; i < scene->mNumLights; i++){
                aiLight * light = scene->mLights[i];
                btVector3 color = convertAiColorToBtVector(light->mColorDiffuse);
                btVector3 lightPosition = convertAiVectorToBtVector(light->mPosition);
                BlLightPoint *lightPoints = new BlLightPoint(position + lightPosition
                                , color, light->mAttenuationConstant, light->mAttenuationLinear,
                                light->mAttenuationQuadratic);
                res.push_back(lightPoints);
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
        xmlNode *cur = node->xmlChildrenNode;
        while (cur != NULL) {
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"position"))) {
                        position = readPositionNode(cur);
                }
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"image"))) {
                        const char *attr = (const char *)xmlGetProp(cur,
                                        (const unsigned char *)"path");
                        image = strduplicate(attr);
                }
                getNodeFloat(cur, "mass", &mass);
                cur = cur->next;
        }
        return loadModelFile(modelPath, position, mass, image);
}

BlLightAmbient *loadAmbientNode(xmlNode *node)
{
        const char *file = (const char *)xmlGetProp(node,
                        (const unsigned char *)"filename");
        return loadAmbientFile(file);
}

std::vector<BlLightPoint*> loadLightNode(xmlNode *node)
{
        const char *modelPath = (const char *)xmlGetProp(node,
                        (const unsigned char *)"filename");
        btVector3 position = btVector3();
        xmlNode *cur = node->xmlChildrenNode;
        while (cur != NULL) {
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"position"))) {
                        position = readPositionNode(cur);
                }
                cur = cur->next;
        }
        return loadLightFile(modelPath, position);
}

BlScene *loadXmlScene(const char *filename)
{
        xmlDoc         *document;
        xmlNode        *root, *node;

        std::vector<BlModel*> *models = new std::vector<BlModel*>();
        std::vector<BlLightPoint*> *lights = new std::vector<BlLightPoint*>();
        BlLightAmbient* ambient = new BlLightAmbient(btVector3());

        document = xmlReadFile(filename, NULL, 0);
        root = xmlDocGetRootElement(document);
        for (node = root->children; node; node = node->next) {
                if(strcmp((char *)node->name, "model") == 0) {
                        std::vector<BlModel*> nodeModels = loadModelNode(node);
                        for (unsigned int i = 0; i < nodeModels.size(); i++) {
                                models->push_back(nodeModels[i]);
                        }
                }
                if(strcmp((char *)node->name, "light") == 0) {
                        std::vector<BlLightPoint*> nodeLights = loadLightNode(node);
                        for (unsigned int i = 0; i < nodeLights.size(); i++) {
                                lights->push_back(nodeLights[i]);
                        }
                }
                if(strcmp((char *)node->name, "ambient") == 0) {
                        ambient = loadAmbientNode(node);
                }
        }
        return new BlScene(models, lights, ambient);
}
