#include "bl_simulation.h"
#include <bl_log.h>
#include <bl_shape.h>

BlSimulation::BlSimulation()
{
        collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,
                        solver,collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

BlSimulation::~BlSimulation()
{
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

void BlSimulation::step(void)
{
        dynamicsWorld->stepSimulation(1.f/60.f);
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
