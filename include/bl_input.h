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
        double lastTicks;
        btVector3 position;
        int axisX;
        int axisY;

        public:
                BlInput();
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
                void handleDown(SDL_Event *event);
                void handleUp(SDL_Event *event);
};

#endif
