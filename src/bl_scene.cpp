#include "bl_scene.h"

BlScene::~BlScene()
{
        for (std::vector<BlModel*>::iterator it = blModels->begin();
                        it != blModels->end(); ++it) {
                delete (*it);
        }
        delete blModels;
        for (std::vector<BlLightPoint*>::iterator it = blLightPoints->begin();
                        it != blLightPoints->end(); ++it) {
                delete (*it);
        }
        delete blLightPoints;
        delete blLightAmbient;
}
