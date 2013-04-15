#include "bl_simulation.h"

BlSimulation::BlSimulation()
{
        collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0, -10, 0));

        initGround();
        initBox();
}

BlSimulation::~BlSimulation()
{
        delete dynamicsWorld;
        delete solver;
        delete overlappingPairCache;
        delete dispatcher;
        delete collisionConfiguration;
}

void BlSimulation::addBody(btCollisionShape *colShape
                , btTransform transform, btScalar mass)
{
        bool isDynamic = (mass != 0.f);
        btVector3 localInertia(0,0,0);
        if(isDynamic)
                colShape->calculateLocalInertia(mass, localInertia);
        btDefaultMotionState *motionState = new btDefaultMotionState(transform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass
                        , motionState, colShape, localInertia);
        btRigidBody *body = new btRigidBody(rbInfo);
        dynamicsWorld->addRigidBody(body);
}

void BlSimulation::initBox()
{
        btCollisionShape *colShape = new btSphereShape(btScalar(1.));
        collisionShapes.push_back(colShape);

        btTransform startTransform;
        startTransform.setIdentity();
        startTransform.setOrigin(btVector3(2,10,0));
        btScalar mass(1.f);

        addBody(colShape, startTransform, mass);
}

void BlSimulation::initGround()
{
        btCollisionShape *colShape = new btBoxShape(btVector3(btScalar(50.)
                                , btScalar(50.), btScalar(50.)));
        collisionShapes.push_back(colShape);
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -56, 0));

        btScalar mass(0.);

        addBody(colShape, groundTransform, mass);
}

void BlSimulation::doSimulation()
{
        for(int i = 0; i < 100; i++)
        {
                dynamicsWorld->stepSimulation(1.f/60.f, 10);
                for(int j = dynamicsWorld->getNumCollisionObjects()-1; j>=0; j--) {
                        btCollisionObject *obj = dynamicsWorld->getCollisionObjectArray()[j];
                        btRigidBody *body = btRigidBody::upcast(obj);
                        if(body && body->getMotionState()) {
                                btTransform trans;
                                body->getMotionState()->getWorldTransform(trans);
                                printf("word pos = %f,%f,%f\n"
                                        , float(trans.getOrigin().getX())
                                        , float(trans.getOrigin().getY())
                                        , float(trans.getOrigin().getZ()));
                        }
                }
        }

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
