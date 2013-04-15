#ifndef BL_SIMULATION_H
#define BL_SIMULATION_H

#include "btBulletDynamicsCommon.h"

class BlSimulation {
        public:
                BlSimulation(void);
                void doSimulation();
                virtual ~BlSimulation();
        private:
                btAlignedObjectArray<btCollisionShape*> collisionShapes;
                btDiscreteDynamicsWorld* dynamicsWorld;
                btDefaultCollisionConfiguration *collisionConfiguration;
                btCollisionDispatcher *dispatcher;
                btBroadphaseInterface *overlappingPairCache;
                btSequentialImpulseConstraintSolver *solver;
                void initBox();
                void initGround();
                void addBody(btCollisionShape *colShape
                                , btTransform transform, btScalar mass);

};

#endif
