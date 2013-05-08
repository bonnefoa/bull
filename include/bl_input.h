#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

#include <bullet.h>
#include <SDL.h>

enum gamestate {
        NORMAL,
        QUIT,
        RELOAD,
        STOP,
};

btVector3 convertCoordinate(btScalar theta, btScalar phi);

class BlInput
{
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

        int sAxisRight;
        int sAxisLeft;
        int sAxisUp;
        int sAxisDown;

        Uint32 lastTicks;
        btVector3 direction;
        btVector3 right;
        btVector3 up;

        public:
                BlInput();
                void handleInput();

                float phi;
                float theta;
                Uint32 now;
                int state;

                btTransform view;
                btTransform projection;

                btTransform computeView(const btVector3 &right
                                , const btVector3 &up
                                , const btVector3 &direction
                                , const btVector3 &position);

                btTransform computeProjection(btScalar fov, btScalar aspect
                                , btScalar zNear, btScalar zFar);
                void logState();
                void handleMovement();

        private:
                float getDeltaTime();
                void computeNewAngles(float deltaTime);
                void handleDown(SDL_Event *event);
                void handleUp(SDL_Event *event);
};

#endif
