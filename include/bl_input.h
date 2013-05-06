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
        btVector3 direction;
        btVector3 right;

        public:
                BlInput();
                void handleInput();

                Uint32 now;
                int gameState;

                btTransform view;
                btTransform projection;

                btTransform computeView(const btVector3 &lookAt
                                , btVector3 &right
                                , const btVector3 &position);
                btTransform computeProjection(btScalar fov, btScalar aspect
                                , btScalar zNear, btScalar zFar);
                void logState();
        private:
                btVector3 computeCurrentDirection();
                void handleMovement();
                float getDeltaTime();
                void computeNewAngles(float deltaTime);
                void handleDown(SDL_Event *event);
                void handleUp(SDL_Event *event);
};

#endif
