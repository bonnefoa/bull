#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

#include <bullet.h>
#include <SDL.h>

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
        btVector3 position;
        int axisRight;
        int axisLeft;
        int axisUp;
        int axisDown;

        Uint32 lastTicks;
        Uint32 nextTime;
        Uint32 now;

        public:
                BlInput();
                void handleInput();

                int gameState;

                btTransform view;
                btTransform projection;

                btTransform computeView(const btVector3 &lookAt
                                , btVector3 &right
                                , const btVector3 &position);
                btTransform computeProjection(btScalar fov, btScalar aspect
                                , btScalar zNear, btScalar zFar);
        private:
                btVector3 computeCurrentDirection();
                void handleMovement();
                float getDeltaTime();
                void computeNewAngles(float deltaTime);
                void handleDown(SDL_Event *event);
                void handleUp(SDL_Event *event);
};

#endif
