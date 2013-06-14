#include "bl_character.h"
#include <bl_matrix.h>
#include <bl_physic.h>
#include <bl_log.h>
#include <bl_util.h>

BlCharacter::BlCharacter(std::vector<BlModel*> *_blModels,
                float mass,
                float linearDamping,
                float angularDamping,
                float angularThreshold,
                float speed,
                btConvexShape *_shape,
                BlState* _blState,
                btTransform transform
                )
: blModels(_blModels),
        shape(_shape),
        blState(_blState),
        angularThreshold(angularThreshold),
        speed(speed)
{
        rigidBody = buildRigidBody(mass, shape, transform);
        rigidBody->setAngularFactor(btVector3(0,1,0));
        rigidBody->setDamping(linearDamping, angularDamping);
}

void BlCharacter::handleTurn(const btQuaternion &direction)
{
        btTransform tr;
        rigidBody->getMotionState()->getWorldTransform(tr);

        btQuaternion persRot = direction.nearest(tr.getRotation());
        btQuaternion delta = persRot.inverse() * direction;

        btVector3 ang = rigidBody->getAngularVelocity();

        if(ang[1] * delta.getAxis()[1] < 0) {
                rigidBody->setAngularVelocity(btVector3(0,0,0));
        }

        float diff = delta.getAngle();
        if(fabs(diff) < angularThreshold) {
                return;
        }
        rigidBody->applyTorqueImpulse(delta.getAxis() / 2.0f * btVector3(0,1,0));
}

void BlCharacter::handleMovement()
{
        btVector3 rightDirection = getRightDirection();
        btVector3 direction = getDirection();

        btVector3 deltaPosition = blState->getDeltaY()
                * direction * blState->deltaTime * speed;
        deltaPosition += blState->getDeltaX()
                * rightDirection * blState->deltaTime * speed;

        if(deltaPosition == btVector3()){
                return;
        }

        rigidBody->forceActivationState(ACTIVE_TAG);
        rigidBody->applyCentralForce(deltaPosition);
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

btVector3 BlCharacter::getRightDirection()
{
        btTransform tr;
        rigidBody->getMotionState()->getWorldTransform(tr);
        btQuaternion rot = tr.getRotation();
        btTransform unitTrans = btTransform(rot);
        return unitTrans(btVector3(1,0,0));
}

btVector3 BlCharacter::getDirection()
{
        btTransform tr;
        rigidBody->getMotionState()->getWorldTransform(tr);
        btQuaternion rot = tr.getRotation();
        btTransform unitTrans = btTransform(rot);
        return unitTrans(btVector3(0,0,-1));
}

btVector3 BlCharacter::getPosition()
{
        btTransform trans;
        rigidBody->getMotionState()->getWorldTransform(trans);
        return trans.getOrigin();
}
