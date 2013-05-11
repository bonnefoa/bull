#ifndef BL_SCENE_H
#define BL_SCENE_H

#include <bl_model.h>
#include <bl_light.h>

class BlScene {
        public:
                BlScene (std::vector<BlModel*> * _models
                                , std::vector<BlLight*> *_lights) :
                        blModels(_models), blLights(_lights) {} ;

                std::vector<BlModel*> *blModels;
                std::vector<BlLight*> *blLights;
        virtual ~BlScene ();
};

#endif
