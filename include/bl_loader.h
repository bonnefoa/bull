#ifndef BL_LOADER_H
#define BL_LOADER_H

#include <stdio.h>
#include <string.h>
#include <vector>
#include <bl_model.h>
#include <bl_terrain.h>
#include <bl_scene.h>
#include <bl_mesh_loader.h>
#include <yaml.h>

class BlLoader
{
        public:
                BlLoader(BlTexture *_blTexture) :
                        blTexture(_blTexture),
                        blMeshLoader(BlMeshLoader(_blTexture))
                                { } ;
                ~BlLoader();

                BlScene *loadScene(const char *filename);

        private:
                BlTexture *blTexture;
                BlMeshLoader blMeshLoader;
                std::vector<BlLightPoint*> loadLightNode(YAML::Node node);
                BlLightAmbient *loadAmbientNode(YAML::Node node);
                std::vector<BlModel*> loadModel(YAML::Node node);
                BlTerrain* loadTerrain(YAML::Node node);
                btRigidBody *readShapeNode(YAML::Node node, btVector3 position);
                btCollisionShape *readCollisionShape(YAML::Node node);
                btTransform readShapeTransform(YAML::Node node, btVector3 position);
                btVector3 readVector3(YAML::Node node);
};


#endif
