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
        rigidBody->setAngularFactor(btVector3(0,0,0));
}

void BlCharacter::handleMovement()
{
        btTransform tr;
        btVector3 deltaPosition = blState->getDeltaPosition();
        rigidBody->setLinearVelocity(deltaPosition);

        rigidBody->getMotionState()->getWorldTransform(tr);
        blState->position = tr.getOrigin() - blState->direction * 2;
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
