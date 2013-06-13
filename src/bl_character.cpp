#include "bl_character.h"
#include <bl_matrix.h>
#include <bl_physic.h>
#include <bl_log.h>
#include <bl_util.h>

BlCharacter::BlCharacter(std::vector<BlModel*> *_blModels,
                float mass,
                btConvexShape *_shape,
                BlState* _blState,
                btTransform transform
                )
: blModels(_blModels), shape(_shape), blState(_blState)
{
        rigidBody = buildRigidBody(mass, shape, transform);
        rigidBody->setAngularFactor(btVector3(0,1,0));
        rigidBody->setDamping(0.5f, 0.5f);
}

void BlCharacter::handleRotation()
{
        btTransform tr;
        rigidBody->getMotionState()->getWorldTransform(tr);

        btQuaternion rot = blState->rotation;
        btQuaternion persRot = rot.nearest(tr.getRotation());
        btQuaternion delta = persRot.inverse() * rot;

        btVector3 ang = rigidBody->getAngularVelocity();

        if(ang[1] * delta.getAxis()[1] < 0) {
                rigidBody->setAngularVelocity(btVector3(0,0,0));
        }

        float diff = delta.getAngle();
        if(fabs(diff) < 0.04f) {
                rigidBody->setAngularVelocity(btVector3(0,0,0));
                return;
        }
        rigidBody->applyTorqueImpulse(delta.getAxis() / 2.0f * btVector3(0,1,0));
}

void BlCharacter::handleMovement()
{
        handleRotation();

        btVector3 deltaPosition = blState->getDeltaPosition();
        if(deltaPosition == btVector3()){
                return;
        }

        btTransform tr;
        rigidBody->getMotionState()->getWorldTransform(tr);

        rigidBody->forceActivationState(ACTIVE_TAG);
        rigidBody->applyCentralForce(deltaPosition);
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

void BlCharacter::drawCharacter(GLint locModel, GLint locVertices,
                GLint locNormals, GLint locUVs)
{
        for (std::vector<BlModel*>::iterator
                        it = blModels->begin();
                        it != blModels->end(); ++it) {
                BlModel *blModel = *it;
                bindModelMatrix(locModel);
                blModel->drawElement(-1, locVertices,
                                locNormals, locUVs);
        }
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
