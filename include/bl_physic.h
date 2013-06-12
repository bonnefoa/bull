#ifndef BL_PHYSIC_H
#define BL_PHYSIC_H

#include <bullet.h>
btRigidBody *buildRigidBody(float mass, btConvexShape *collisionShape,
                btTransform transform);

#endif
