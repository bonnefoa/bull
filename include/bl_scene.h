#ifndef BL_SCENE_H
#define BL_SCENE_H

#include <bl_model.h>
#include <bl_light_point.h>
#include <bl_light_ambient.h>

class BlScene {
        public:
                BlScene (std::vector<BlModel*> * _models
                                , std::vector<BlLightPoint*> *_lightPoints
                                , BlLightAmbient *_ambient
                                ) :
                        blModels(_models),
                        blLightPoints(_lightPoints),
                        blLightAmbient(_ambient)
        {} ;

                std::vector<BlModel*> *blModels;
                std::vector<BlLightPoint*> *blLightPoints;
                BlLightAmbient *blLightAmbient;
        virtual ~BlScene ();
};

#endif
