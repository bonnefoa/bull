#ifndef BL_MESH_LOADER_H
#define BL_MESH_LOADER_H

#include <bullet.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <bl_log.h>
#include <bl_image.h>
#include <bl_light_point.h>
#include <bl_light_ambient.h>
#include <bl_util.h>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <bl_texture.h>

class BlMeshLoader
{
        public:
                BlMeshLoader(BlTexture *_blTexture) :
                        blTexture(_blTexture) {};
                ~BlMeshLoader();

                BlLightAmbient *loadAmbientFile(const char *path);
                std::vector<BlModel*> loadModelFile(const char *modelPath,
                                btVector3 position, btRigidBody *rigidBody,
                                const char *image);
                std::vector<BlLightPoint*> loadLightFile(const char *path,
                                btVector3 position,
                                std::vector<BlModel*> *models);
        private:
                BlTexture *blTexture;
                Assimp::Importer importer;
                btVector3 convertAiColorToBtVector(aiColor3D color);
                btVector3 convertAiVectorToBtVector(aiVector3D vec);
                std::vector <btVector3> loadVertices(aiMesh *mesh);
                std::vector <btVector3> loadVerticesInformation(aiMesh *mesh,
                                aiVector3D *infos);
                std::vector <btVector3> loadNormals(aiMesh *mesh);
                std::vector <unsigned int> loadIndices(aiMesh *mesh);
                std::vector< BlUvs > loadUvs(aiMesh *mesh);
                const aiScene *loadAssimpScene(const char *path);
                void fillConvexShapePoints(std::vector <btVector3> *vertices,
                                btConvexHullShape *colShape);
};

#endif
