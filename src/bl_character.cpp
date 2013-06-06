#include "bl_character.h"

BlCharacter::BlCharacter(std::vector<BlModel*> _blModels,
                btConvexShape *_shape)
: blModels(_blModels), shape(_shape)
{
        ghostObject = new btPairCachingGhostObject();
        controller = new btKinematicCharacterController(ghostObject,
                        shape, 1.0f);
        (void) blModels;
}

BlCharacter::~BlCharacter()
{
        delete controller;
        delete ghostObject;
}
