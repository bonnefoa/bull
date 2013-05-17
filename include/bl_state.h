#ifndef BL_STATE_H
#define BL_STATE_H

#include <SDL.h>
#include <bl_simulation.h>

#define MAX_AXIS 20

enum gamestate_enum {
        NORMAL,
        QUIT,
        RELOAD,
        STOP,
};

class BlState {
        public:
                BlState(btVector3 _position) :
                        gamestate(NORMAL),

                        position(_position),
                        view(btTransform()),

                        lightState(0),
                        debugState(0),

                        axisRight(0),
                        axisLeft(0),
                        axisUp(0),
                        axisDown(0),
                        sAxisRight(0),
                        sAxisLeft(0),
                        sAxisUp(0),
                        sAxisDown(0),
                        leftMouse(0),
                        rightMouse(0) {};

                gamestate_enum gamestate;

                btVector3 position;
                btTransform view;

                int lightState;
                int debugState;

                int axisRight;
                int axisLeft;
                int axisUp;
                int axisDown;

                int sAxisRight;
                int sAxisLeft;
                int sAxisUp;
                int sAxisDown;

                int leftMouse;
                int rightMouse;

                void forward(SDL_Keymod mod);
                void back(SDL_Keymod mod);
                void left(SDL_Keymod mod);
                void right(SDL_Keymod mod);
                void light();
                void pause();

                void stopForward();
                void stopBack();
                void stopLeft();
                void stopRight();
                void stopLight();

                void debug();
                void logState();

        private:
                void incrementAxis(SDL_Keymod mod, int *normalAxis, int *modAxis);
};

#endif
