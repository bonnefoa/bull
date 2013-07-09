#ifndef BL_SCENE_H
#define BL_SCENE_H

#include <bl_model.h>
#include <bl_terrain.h>
#include <bl_light_point.h>
#include <bl_light_ambient.h>
#include <bl_character.h>
#include <bl_simulation.h>
#include <bl_skybox.h>

class BlScene {
        public:
                BlScene (std::vector<BlModel*> * _models
                                , std::vector<BlLightPoint*> *_lightPoints
                                , BlLightAmbient *_ambient
                                , std::vector<BlTerrain*> *_terrains
                                , BlCharacter *_blCharacter
                                , BlSkybox *_blSkybox
                        ) :
                        blModels(_models),
                        blLightPoints(_lightPoints),
                        blLightAmbient(_ambient),
                        blTerrains(_terrains),
                        blCharacter(_blCharacter),
                        blSkybox(_blSkybox) {} ;

                std::vector<BlModel*> *blModels;
                std::vector<BlLightPoint*> *blLightPoints;
                BlLightAmbient *blLightAmbient;
                std::vector<BlTerrain*> *blTerrains;
                BlCharacter *blCharacter;
                BlSkybox *blSkybox;

                virtual ~BlScene ();

                void init(BlSimulation *blSimulation, GLuint programModelId);

};

#endif
