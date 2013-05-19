#include "bl_simulation.h"
#include <bl_log.h>
#include <bl_shape.h>
#include <bl_debug_drawer.h>
#include <bl_program_debug.h>

BlSimulation::BlSimulation(BlConfig *blConfig, BlState *blState)
{
        collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,
                        solver,collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0, -10, 0));

        BlProgramDebug *blProgramDebug = getProgramDebug(blConfig);
        blDebugDrawer = new BlDebugDrawer(blProgramDebug, blState);
        blDebugDrawer->init();
        dynamicsWorld->setDebugDrawer(blDebugDrawer);
}

BlSimulation::~BlSimulation()
{
        delete dynamicsWorld->getDebugDrawer();
        delete dynamicsWorld;
        delete solver;
        delete overlappingPairCache;
        delete dispatcher;
        delete collisionConfiguration;
}

void BlSimulation::addBlModel(BlModel *blModel)
{
        if(blModel->rigidBody) {
                dynamicsWorld->addRigidBody(blModel->rigidBody);
        }
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
        if(dynamicsWorld->getDebugDrawer()->getDebugMode() > 0) {
                blDebugDrawer->initDebugRender();
                dynamicsWorld->debugDrawWorld();
        }
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
