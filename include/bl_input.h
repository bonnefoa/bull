#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

#include <bullet.h>
#include <SDL.h>
#include <bl_state.h>

btVector3 convertCoordinate(btScalar theta, btScalar phi);

class BlInput
{

        public:
                BlInput(BlState *blState);
                void handleInput();

                float phi;
                float theta;
                Uint32 now;

                int lDown;
                int leftMouse;
                int rightMouse;

                btVector3 position;
                btTransform view;
                btTransform projection;

                void logState();
                void handleMovement();

        private:
                float getDeltaTime();
                void computeNewAngles(float deltaTime);
                void handleKeyDown(SDL_Event *event);
                void handleKeyUp(SDL_Event *event);
                void handleMouseDown(SDL_Event *event);
                void handleMouseUp(SDL_Event *event);


                float mouseSpeed;
                float speed;
                btScalar fov;
                btScalar aspect;
                btScalar zNear;
                btScalar zFar;

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
                BlState *blState;
};

#endif
