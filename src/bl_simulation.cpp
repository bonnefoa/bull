#include "bl_simulation.h"
#include <bl_log.h>
#include <bl_shape.h>
#include <bl_debug_drawer.h>
#include <bl_program_debug.h>
#include <bl_matrix.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseProxy.h>
#include <bl_character.h>

BlSimulation::BlSimulation(BlDebugDrawer *_blDebugDrawer
                , BlState *_blState)
: blDebugDrawer(_blDebugDrawer), blState(_blState)
{
        collisionConfiguration = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfiguration);
        overlappingPairCache = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver;
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,
                        solver,collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0, -10, 0));
        dynamicsWorld->setDebugDrawer(_blDebugDrawer);
        pickedBody = NULL;
}

BlSimulation::~BlSimulation()
{
        delete dynamicsWorld;
        delete solver;
        delete overlappingPairCache;
        delete dispatcher;
        delete collisionConfiguration;
}

btCollisionWorld::ClosestRayResultCallback BlSimulation::getCenterObject(BlCharacter *blCharacter)
{
        btVector3 position = blCharacter->getPosition();
        btVector3 direction = blCharacter->getDirection();
        btCollisionWorld::ClosestRayResultCallback rayCallback(position,
                        direction);
        dynamicsWorld->rayTest(position, direction * 100, rayCallback);
        return rayCallback;
}

void BlSimulation::endPickObject(BlCharacter *blCharacter)
{
        (void) blCharacter;
        if(!pickedBody) {
                return;
        }
        pickedBody->forceActivationState(ACTIVE_TAG);
        pickedBody->setDeactivationTime(0.f);
        pickedBody->setGravity(btVector3(0,-10,0));
        pickedBody = NULL;
}

void BlSimulation::movePickedObject(BlCharacter *blCharacter)
{
        btVector3 position = blCharacter->getPosition()
                + blCharacter->getDirection() * 3;
        btTransform tr = buildModelMatrix(btVector3(1,1,1), position);
        pickedBody->setCenterOfMassTransform(tr);
}

void BlSimulation::initializePickedObject(
                btCollisionWorld::ClosestRayResultCallback rayCallback,
                BlCharacter *blCharacter)
{
        pickedBody = btRigidBody::upcast(
                        rayCallback.m_collisionObject);
        if(pickedBody->isStaticObject()) {
                pickedBody = NULL;
                return;
        }

        btVector3 position = blCharacter->getPosition()
                + blCharacter->getDirection() * 3;
        btTransform tr = buildModelMatrix(btVector3(1,1,1), position);

        pickedBody->setGravity(btVector3(0,0,0));
        pickedBody->setCenterOfMassTransform(tr);
        pickedBody->activate(true);
        pickedBody->setActivationState(DISABLE_DEACTIVATION);
}

void BlSimulation::pickObject(BlCharacter *blCharacter)
{
        if(pickedBody) {
                movePickedObject(blCharacter);
                return;
        }
        btCollisionWorld::ClosestRayResultCallback rayCallback =
                getCenterObject(blCharacter);
        if(!rayCallback.hasHit()) {
                return;
        }
        initializePickedObject(rayCallback, blCharacter);
}

void BlSimulation::pushObject(BlCharacter *blCharacter)
{
        btCollisionWorld::ClosestRayResultCallback rayCallback =
                getCenterObject(blCharacter);
        if(rayCallback.hasHit()) {
                btRigidBody *body = btRigidBody::upcast(
                                rayCallback.m_collisionObject);
                if(!body->isStaticObject()) {
                        btVector3 str = blCharacter->getDirection();
                        body->forceActivationState(ACTIVE_TAG);
                        body->applyCentralImpulse(str);
                }
        }
}

void BlSimulation::addRigidBody(btRigidBody *rigidBody)
{
        if(rigidBody) {
                dynamicsWorld->addRigidBody(rigidBody);
        }
}

void BlSimulation::addCharacter(BlCharacter *blCharacter)
{
        dynamicsWorld->addRigidBody(blCharacter->rigidBody);
}


void BlSimulation::toggleDebug(int debugState)
{
        if (debugState==0) {
                dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_NoDebug);
        } else {
                INFO("Toggle debug on");
                dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
        }
}

void BlSimulation::step(void)
{
        dynamicsWorld->stepSimulation(1.f/60.f);
}

void BlSimulation::debugDraw()
{
        dynamicsWorld->debugDrawWorld();
}

void BlSimulation::clearWorld()
{
        for(int i = dynamicsWorld->getNumCollisionObjects()-1; i>=0; i--) {
                btCollisionObject *obj = dynamicsWorld->getCollisionObjectArray()[i];
                btRigidBody *body = btRigidBody::upcast(obj);
                if(body && body->getMotionState()) {
                        delete body->getMotionState();
                }
                dynamicsWorld->removeCollisionObject(obj);
                delete obj;
        }
        for(int i=0; i<collisionShapes.size();i++) {
                btCollisionShape *shape = collisionShapes[i];
                collisionShapes[i] = 0;
                delete shape;
        }
}
