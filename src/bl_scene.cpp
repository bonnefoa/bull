#include "bl_scene.h"

void BlScene::init(BlSimulation *blSimulation, GLuint programModelId)
{
        for (std::vector<BlModel*>::iterator it = blModels->begin();
                        it != blModels->end(); ++it) {
                BlModel *model = *it;
                model->init();
                model->loadInBuffer();
                blSimulation->addRigidBody(model->rigidBody);
        }
        for (std::vector<BlTerrain*>::iterator
                        it = blTerrains->begin();
                        it != blTerrains->end(); ++it) {
                BlTerrain *terrain = *it;
                terrain->init();
                terrain->loadInBuffer();
                blSimulation->addRigidBody(terrain->rigidBody);
        }
        for (std::vector<BlLightPoint*>::iterator it = blLightPoints->begin();
                        it != blLightPoints->end(); ++it) {
                BlLightPoint *light = *it;
                light->init();
                light->loadInBuffer(programModelId);
        }
        blLightAmbient->loadInBuffer(programModelId);

        blCharacter->loadInBuffer();
        blSimulation->addCharacter(blCharacter);
}

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

        for (std::vector<BlTerrain*>::iterator it = blTerrains->begin();
                        it != blTerrains->end(); ++it) {
                delete (*it);
        }
        delete blTerrains;
        delete blCharacter;
}
