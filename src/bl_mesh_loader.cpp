#include "bl_mesh_loader.h"

Assimp::Importer importer;

btVector3 convertAiColorToBtVector(aiColor3D color)
{
        return btVector3(color[0], color[1], color[2]);
}

btVector3 convertAiVectorToBtVector(aiVector3D vec)
{
        return btVector3(vec.x, vec.z, -vec.y);
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

const aiScene *loadAssimpScene(const char *path)
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

BlLightAmbient *loadAmbientFile(const char *path)
{
        INFO("Loading ambient from file %s\n", path);
        const aiScene *scene = loadAssimpScene(path);
        if (scene->mNumLights > 0 ) {
                aiLight * light = scene->mLights[0];
                btVector3 color = convertAiColorToBtVector(light->mColorDiffuse);
                return new BlLightAmbient(color);
        }
        return new BlLightAmbient(btVector3());
}

std::vector<BlLightPoint*> loadLightFile(const char *path,
                btVector3 position, std::vector<BlModel*> *models)
{
        INFO("Loading light from file %s, position %f %f %f\n",
                        path, position[0], position[1], position[2]);
        std::vector<BlLightPoint*> res = std::vector<BlLightPoint*>();
        const aiScene *scene = loadAssimpScene(path);
        for (unsigned int i = 0; i < scene->mNumLights; i++){
                aiLight * light = scene->mLights[i];
                btVector3 color = convertAiColorToBtVector(light->mColorDiffuse);
                btVector3 lightPosition = convertAiVectorToBtVector(light->mPosition);
                INFO("Loading light point, color %f %f %f, attenuation : 1 / (%f + d * %f + d * d * %f)\n",
                                path, color[0], color[1], color[2],
                                light->mAttenuationConstant,
                                light->mAttenuationLinear,
                                light->mAttenuationQuadratic);
                BlLightPoint *lightPoints = new BlLightPoint(
                                position + lightPosition,
                                color, light->mAttenuationConstant,
                                light->mAttenuationLinear,
                                light->mAttenuationQuadratic,
                                models);
                res.push_back(lightPoints);
        }
        return res;
}

std::vector<BlModel*> loadModelFile(const char *modelPath,
                btVector3 position, float mass, const char *image)
{
        INFO("Loading asset from file %s, mass %f, image %s, position %f %f %f\n",
                        modelPath, mass, image,
                        position[0], position[1], position[2]);
        std::vector<BlModel*> res = std::vector<BlModel*>();
        const aiScene *scene = loadAssimpScene(modelPath);

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
