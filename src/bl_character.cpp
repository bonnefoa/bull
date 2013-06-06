#include "bl_character.h"

BlCharacter::BlCharacter(std::vector<BlModel*> *_blModels,
                btConvexShape *_shape)
: blModels(_blModels), shape(_shape)
{
        ghostObject = new btPairCachingGhostObject();
        ghostObject->setCollisionShape(shape);
        ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
        btScalar stepHeight = 0.35;
        controller = new btKinematicCharacterController(ghostObject,
                        shape, stepHeight);
        (void) blModels;
}

BlCharacter::~BlCharacter()
{
        delete controller;
        delete ghostObject;
}

void BlCharacter::loadInBuffer()
{
        for (std::vector<BlModel*>::iterator it = blModels->begin();
                        it != blModels->end(); ++it) {
                (*it)->init();
                (*it)->loadInBuffer();
        }
}
