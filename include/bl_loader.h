#ifndef BL_LOADER_H
#define BL_LOADER_H

#include <stdio.h>
#include <string.h>
#include <vector>
#include <bl_model.h>
#include <bl_terrain.h>
#include <bl_scene.h>
#include <bl_mesh_loader.h>
#include <bl_character.h>
#include <bl_state.h>
#include <bl_skybox.h>
#include <bl_yaml.h>

class BlLoader
{
        public:
                BlLoader(BlTexture *_blTexture, BlState *_blState) :
                        blTexture(_blTexture),
                        blMeshLoader(BlMeshLoader(_blTexture)),
                        blState(_blState)
                                { } ;
                ~BlLoader();

                BlScene *loadScene(const char *filename);

        private:
                BlTexture *blTexture;
                BlMeshLoader blMeshLoader;
                BlState *blState;
                std::vector<BlLightPoint*> loadLightNode(YAML::Node node);
                BlLightAmbient *loadAmbientNode(YAML::Node node);
                std::vector<BlModel*> *loadModel(YAML::Node node);
                BlTerrain* loadTerrain(YAML::Node node);
                std::map<std::string, btRigidBody*> readShapeNode(
                                YAML::Node node,
                                btVector3 position);
                btConvexShape *readCollisionShape(YAML::Node node);
                btTransform readShapeTransform(
                                YAML::Node node, btVector3 position);
                std::map<std::string, btVector3> readShapeOffset(YAML::Node node);
                BlCharacter *loadCharacter(YAML::Node node);
                BlSkybox *loadSkybox(YAML::Node node) ;
};


#endif
