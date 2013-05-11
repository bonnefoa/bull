#include "bl_scene.h"

BlScene::~BlScene()
{
        for (std::vector<BlModel*>::iterator it = blModels->begin();
                        it != blModels->end(); ++it) {
                (*it)->clear();
                delete (*it);
        }
        blModels->clear();
}
