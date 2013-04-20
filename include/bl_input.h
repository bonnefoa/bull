#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

#include "btBulletDynamicsCommon.h"

class BlInput
{
        float phi;
        float theta;
        float mouseSpeed;
        float speed;
        btScalar fov;
        btScalar aspect;
        btScalar zNear;
        btScalar zFar;
        double lastTicks;
        bool keys[322];
        btVector3 position;

        public:
                BlInput();
                void pollInput();
                void handleInput();

                int gameState;

                btTransform view;
                btTransform projection;
        private:
                btVector3 computeCurrentDirection();
                void handleMovement();
                float getDeltaTime();
                void computeNewAngles(float deltaTime);
                void computeView(const btVector3 &lookAt
                                , const btVector3 &right
                                , const btVector3 &up
                                , const btVector3 &position);
                void computeProjection();
};

#endif
