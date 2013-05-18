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

btRigidBody *BlSimulation::addBody(btCollisionShape *colShape
                , btTransform transform, btScalar mass)
{
        bool isDynamic = (mass != 0.f);
        btVector3 localInertia(0,0,0);
        btDefaultMotionState *motionState = NULL;
        if(isDynamic) {
                colShape->calculateLocalInertia(mass, localInertia);
                motionState = new btDefaultMotionState(transform);
        }
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass
                        , motionState, colShape, localInertia);
        btRigidBody *body = new btRigidBody(rbInfo);
        dynamicsWorld->addRigidBody(body);
        return body;
}

btRigidBody *BlSimulation::addBlModel(BlModel *blModel)
{
                btTransform startTransform;
                startTransform.setIdentity();
                startTransform.setOrigin(blModel->position);
                btCollisionShape *shape = guessShape(blModel);
                btVector3 center;
                btScalar radius;
                shape->getBoundingSphere(center, radius);
                center += blModel->position;
                INFO("Add rigid body for %s with mass %f, shape %s, position %f %f %f\n",
                                blModel->name, blModel->mass,
                                shape->getName(), blModel->position[0],
                                blModel->position[1], blModel->position[2]);
                INFO("bouding sphere center %f %f %f, radius %f\n",
                                center[0], center[1], center[2], radius);
                return addBody(shape, startTransform, blModel->mass);
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
