#ifndef BL_SIMULATION_H
#define BL_SIMULATION_H

#include <bullet.h>
#include <bl_model.h>
#include <bl_state.h>
#include <bl_config.h>
#include <bl_debug_drawer.h>

class BlSimulation {
        public:
                BlSimulation(BlConfig *blConfig, BlState *blState);
                void doSimulation();
                virtual ~BlSimulation();
                void step(void);
                void addRigidBody(btRigidBody *rigidBody);
                void clearWorld(void);
                void toggleDebug(int debugState);
                void debugDraw();
                int getDebugState();

        private:
                BlState *blState;
                BlDebugDrawer *blDebugDrawer;

                btAlignedObjectArray<btCollisionShape*> collisionShapes;
                btDiscreteDynamicsWorld* dynamicsWorld;
                btDefaultCollisionConfiguration *collisionConfiguration;
                btCollisionDispatcher *dispatcher;
                btBroadphaseInterface *overlappingPairCache;
                btSequentialImpulseConstraintSolver *solver;

};

#endif
