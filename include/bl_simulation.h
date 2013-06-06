#ifndef BL_SIMULATION_H
#define BL_SIMULATION_H

#include <bullet.h>
#include <bl_model.h>
#include <bl_state.h>
#include <bl_config.h>
#include <bl_debug_drawer.h>

class BlSimulation {
        public:
                BlSimulation(BlDebugDrawer *blDebugDrawer,
                                BlState *_blState);
                void doSimulation();
                virtual ~BlSimulation();
                void step(void);
                void addRigidBody(btRigidBody *rigidBody);
                void clearWorld(void);
                void toggleDebug(int debugState);
                void debugDraw();

                void pushObject();
                void pickObject();
                void endPickObject();

        private:
                BlDebugDrawer *blDebugDrawer;
                BlState *blState;
                btRigidBody *pickedBody;

                btAlignedObjectArray<btCollisionShape*> collisionShapes;
                btDiscreteDynamicsWorld* dynamicsWorld;
                btDefaultCollisionConfiguration *collisionConfiguration;
                btCollisionDispatcher *dispatcher;
                btBroadphaseInterface *overlappingPairCache;
                btSequentialImpulseConstraintSolver *solver;

                btCollisionWorld::ClosestRayResultCallback getCenterObject();
                void movePickedObject();
                void initializePickedObject(btCollisionWorld::ClosestRayResultCallback rayCallback);
};

#endif
