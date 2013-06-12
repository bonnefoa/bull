#include "bl_physic.h"

btRigidBody *buildRigidBody(float mass, btConvexShape *collisionShape,
                btTransform transform)
{
        bool isDynamic = (mass != 0.f);
        btVector3 localInertia(0,0,0);
        btDefaultMotionState *motionState = NULL;
        if(isDynamic) {
                collisionShape->calculateLocalInertia(mass, localInertia);
                motionState = new btDefaultMotionState(transform);
        }
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass
                        , motionState, collisionShape, localInertia);
        btRigidBody *body = new btRigidBody(rbInfo);
        if(!isDynamic) {
                body->proceedToTransform(transform);
        }
        return body;
}
