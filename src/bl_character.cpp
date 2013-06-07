#include "bl_character.h"
#include <bl_matrix.h>
#include <bl_physic.h>

BlCharacter::BlCharacter(std::vector<BlModel*> *_blModels,
                float mass,
                btConvexShape *_shape,
                BlState* _blState,
                btTransform transform
                )
: blModels(_blModels), shape(_shape), blState(_blState)
{
        (void) blModels;
        (void) blState;
        rigidBody = buildRigidBody(mass, shape, transform);
}

void BlCharacter::handleMovement()
{
}

void BlCharacter::loadInBuffer()
{
        for (std::vector<BlModel*>::iterator it = blModels->begin();
                        it != blModels->end(); ++it) {
                (*it)->init();
                (*it)->loadInBuffer();
        }
}

void BlCharacter::bindModelMatrix(GLint uniformM)
{
        btTransform trans;
        rigidBody->getMotionState()->getWorldTransform(trans);
        sendTransform(trans, uniformM);
}

BlCharacter::~BlCharacter()
{
        delete rigidBody;
        for (std::vector<BlModel*>::iterator it = blModels->begin();
                        it != blModels->end(); ++it) {
                delete *it;
        }
        delete blModels;
}
