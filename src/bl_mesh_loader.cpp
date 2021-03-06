#include "bl_mesh_loader.h"
#include <BulletCollision/CollisionShapes/btShapeHull.h>

btVector3 BlMeshLoader::convertAiColorToBtVector(aiColor3D color)
{
        return btVector3(color[0], color[1], color[2]);
}

btVector3 BlMeshLoader::convertAiVectorToBtVector(aiVector3D vec)
{
        return btVector3(vec.x, vec.y, vec.z);
}

std::vector <btVector3> BlMeshLoader::loadVertices(aiMesh *mesh,
                btVector3 offset)
{
        std::vector <btVector3> vertices;
        for(unsigned int j=0; j < mesh->mNumVertices; j++){
                vertices.push_back(
                                convertAiVectorToBtVector(mesh->mVertices[j])
                                - offset);
        }
        return vertices;
}

std::vector <btVector3> BlMeshLoader::loadVerticesInformation(aiMesh *mesh,
                aiVector3D *infos)
{
        std::vector <btVector3> result;
        if(infos == NULL) return result;
        for(unsigned int j=0; j < mesh->mNumVertices; j++){
                result.push_back(convertAiVectorToBtVector(infos[j]));
        }
        return result;
}

std::vector <btVector3> BlMeshLoader::loadNormals(aiMesh *mesh)
{
        std::vector <btVector3> normals;
        for(unsigned int j=0; j < mesh->mNumVertices; j++){
                normals.push_back(convertAiVectorToBtVector(mesh->mNormals[j]));
        }
        return normals;
}

std::vector <unsigned int> BlMeshLoader::loadIndices(aiMesh *mesh)
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

std::vector< BlUvs > BlMeshLoader::loadUvs(aiMesh *mesh)
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

const aiScene *BlMeshLoader::loadAssimpScene(const char *path)
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

BlLightAmbient *BlMeshLoader::loadAmbientFile(const char *path)
{
        INFO("Loading ambient from file %s\n", path);

        const aiScene *scene = loadAssimpScene(path);
        if (scene->mNumLights == 0) {
                return new BlLightAmbient(btVector3());
        }
        aiLight * light = scene->mLights[0];
        btVector3 color = convertAiColorToBtVector(light->mColorDiffuse);
        return new BlLightAmbient(color);
}

std::vector<BlLightPoint*> BlMeshLoader::loadLightFile(const char *path,
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

void BlMeshLoader::fillConvexShapePoints(std::vector <btVector3> *vertices,
                btConvexHullShape *colShape)
{
        btConvexHullShape tmpShape;
        for (std::vector<btVector3>::iterator it = vertices->begin();
                        it != vertices->end(); ++it) {
                tmpShape.addPoint(*it);
        }
        btShapeHull hull = btShapeHull(&tmpShape);
        btScalar margin = tmpShape.getMargin();
        hull.buildHull(margin);
        for (int i=0; i<hull.numVertices(); i++) {
                colShape->addPoint(hull.getVertexPointer()[i]);
        }
}

BlModel *BlMeshLoader::loadMesh(
                aiMesh *mesh,
                const char*name,
                std::vector< std::vector<BlModel*>* > *children,
                btVector3 position,
                std::map<std::string, btRigidBody*> mapIndexBody,
                std::map<std::string, btVector3> mapIndexOffset,
                const char *diffuse,
                const char *normal)
{
        btVector3 offset;
        btRigidBody *rigidBody = NULL;

        if(mapIndexBody.count(name) > 0) {
                rigidBody = mapIndexBody[name];
        }
        if(mapIndexOffset.count(name) > 0) {
                offset = mapIndexOffset[name];
                INFO("Found offset %f %f %f\n",
                                offset[0], offset[1], offset[2]);
        }

        for (std::vector< std::vector<BlModel*>*>::iterator
                        it = children->begin();
                        it != children->end(); ++it) {
                for (std::vector<BlModel*>::iterator
                                it2 = (*it)->begin();
                                it2 != (*it)->end(); ++it2) {
                        BlModel *model = *it2;
                        for (std::vector<btVector3>::iterator
                                        itv = model->vertices.begin();
                                        itv != model->vertices.end(); ++itv) {
                                (*itv) -= offset;
                        }
                }
        }


        std::vector <btVector3> vertices = loadVertices(mesh, offset);
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
        if(rigidBody && rigidBody->getCollisionShape()->getShapeType()
                        == CONVEX_HULL_SHAPE_PROXYTYPE) {
                btConvexHullShape *colShape = (btConvexHullShape*) rigidBody->getCollisionShape();
                fillConvexShapePoints(&vertices, colShape);
        }
        INFO("Got a model %s with %i children\n",
                        name, children->size());
        BlModel *blModel = new BlModel(blTexture,
                        children,
                        vertices, indices, normals,
                        tangents, bitangents,
                        uvs, position, rigidBody,
                        name, diffuse,
                        normal);
        return blModel;
}

std::vector<BlModel*> *BlMeshLoader::loadNode(const aiScene *scene,
                aiNode *node,
                btVector3 position,
                std::map<std::string, btRigidBody*> mapIndexBody,
                std::map<std::string, btVector3> mapIndexOffset,
                const char *diffuse,
                const char *normal )
{
        std::vector< std::vector<BlModel*>* > *children =
                new std::vector< std::vector<BlModel*>* >();
        std::vector<BlModel*> *res = new std::vector<BlModel*>();

        for(unsigned int i = 0; i < node->mNumChildren; i++) {
                aiNode *childNode = node->mChildren[i];
                std::vector<BlModel*> *currentChild =
                        loadNode(scene, childNode,
                                        position,
                                        mapIndexBody,
                                        mapIndexOffset,
                                        diffuse,
                                        normal);
                children->push_back(currentChild);
        }

        const char* name = strduplicate(node->mName.C_Str());
        for (unsigned int i = 0; i < node->mNumMeshes; i++){
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                BlModel *currentModel = loadMesh(mesh,
                                name,
                                children,
                                position,
                                mapIndexBody, mapIndexOffset,
                                diffuse, normal);
                res->push_back(currentModel);
        }
        if(node->mNumMeshes == 0) {
                for (std::vector< std::vector<BlModel*>*>::iterator
                                it = children->begin();
                                it != children->end(); ++it) {
                        for (std::vector<BlModel*>::iterator
                                        it2 = (*it)->begin();
                                        it2 != (*it)->end(); ++it2) {
                                res->push_back(*it2);
                        }
                }
        }
        return res;
}

std::vector<BlModel*> *BlMeshLoader::loadModelFile(const char *modelPath,
                btVector3 position,
                std::map<std::string, btRigidBody*> mapIndexBody,
                std::map<std::string, btVector3> mapIndexOffset,
                const char *diffuse,
                const char *normal)
{
        INFO("Loading asset from file %s, diffuse %s, position %f %f %f\n",
                        modelPath, diffuse,
                        position[0], position[1], position[2]);
        const aiScene *scene = loadAssimpScene(modelPath);
        aiNode *rootNode = scene->mRootNode;
        std::vector<BlModel*> *res = loadNode(scene, rootNode,
                        position, mapIndexBody, mapIndexOffset, diffuse, normal);
        return res;
}
