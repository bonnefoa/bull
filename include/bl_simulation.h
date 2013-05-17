#ifndef BL_SIMULATION_H
#define BL_SIMULATION_H

#include <bullet.h>
#include <bl_model.h>
#include <bl_config.h>

class BlSimulation {
        public:
                BlSimulation(BlConfig *blConfig);
                void doSimulation();
                virtual ~BlSimulation();
                btRigidBody *addBody(btCollisionShape *colShape
                                , btTransform transform, btScalar mass);
                void step(void);
                btRigidBody *addBlModel(BlModel *blModel);
                void clearWorld(void);
                void toggleDebug(int debugState);
                void debugDraw();

        private:
                btAlignedObjectArray<btCollisionShape*> collisionShapes;
                btDiscreteDynamicsWorld* dynamicsWorld;
                btDefaultCollisionConfiguration *collisionConfiguration;
                btCollisionDispatcher *dispatcher;
                btBroadphaseInterface *overlappingPairCache;
                btSequentialImpulseConstraintSolver *solver;

};

#endif
