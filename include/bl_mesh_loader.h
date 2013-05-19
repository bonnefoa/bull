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

BlLightAmbient *loadAmbientFile(const char *path);
std::vector<BlModel*> loadModelFile(const char *modelPath,
                btVector3 position, btRigidBody *rigidBody, const char *image);
std::vector<BlLightPoint*> loadLightFile(const char *path,
                btVector3 position, std::vector<BlModel*> *models);
BlLightAmbient *loadAmbientFile(const char *path);

#endif
