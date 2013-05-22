#ifndef BL_STATE_H
#define BL_STATE_H

#include <SDL.h>
#include <bullet.h>

#define MAX_AXIS 20

enum gamestate_enum {
        NORMAL,
        QUIT,
        RELOAD,
        RELOAD_KEEP_STATE,
        STOP,
};

class BlState {
        public:
                BlState(btVector3 _position) :
                        gamestate(NORMAL),

                        phi(0.0f),
                        theta(M_PI_2),
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

                float phi;
                float theta;

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
                void reload(SDL_Keymod mod);
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
