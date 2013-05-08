#ifndef BL_SIMULATION_H
#define BL_SIMULATION_H

#include <bullet.h>
#include <bl_model.h>

class BlSimulation {
        public:
                BlSimulation(void);
                void doSimulation();
                virtual ~BlSimulation();
                btRigidBody *addBody(btCollisionShape *colShape
                                , btTransform transform, btScalar mass);
                void step(void);
                void addBlModel(BlModel *blModel);
                void clearWorld(void);
        private:
                btAlignedObjectArray<btCollisionShape*> collisionShapes;
                btDiscreteDynamicsWorld* dynamicsWorld;
                btDefaultCollisionConfiguration *collisionConfiguration;
                btCollisionDispatcher *dispatcher;
                btBroadphaseInterface *overlappingPairCache;
                btSequentialImpulseConstraintSolver *solver;

};

#endif
