#ifndef BL_SCENE_H
#define BL_SCENE_H

#include <bl_model.h>
#include <bl_terrain.h>
#include <bl_light_point.h>
#include <bl_light_ambient.h>
#include <bl_character.h>
#include <bl_simulation.h>

class BlScene {
        public:
                BlScene (std::vector<BlModel*> * _models
                                , std::vector<BlLightPoint*> *_lightPoints
                                , BlLightAmbient *_ambient
                                , std::vector<BlTerrain*> *_terrains
                                , BlCharacter *_blCharacter
                                ) :
                        blModels(_models),
                        blLightPoints(_lightPoints),
                        blLightAmbient(_ambient),
                        blTerrains(_terrains),
                        blCharacter(_blCharacter)
        {} ;

                std::vector<BlModel*> *blModels;
                std::vector<BlLightPoint*> *blLightPoints;
                BlLightAmbient *blLightAmbient;
                std::vector<BlTerrain*> *blTerrains;
                BlCharacter *blCharacter;
        virtual ~BlScene ();

        void init(BlSimulation *blSimulation, GLuint programModelId);

};

#endif
