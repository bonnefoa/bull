#ifndef BL_STATE_H
#define BL_STATE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <bullet.h>
#include <bl_config.h>

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
                BlState(btVector3 _position, TTF_Font *_font,
                                BlConfig *_blConfig) :
                        gamestate(NORMAL),
                        font(_font),
                        blConfig(_blConfig),

                        phi(0.0f),
                        theta(M_PI_2),
                        position(_position),
                        direction(btVector3(0,0,0)),
                        view(btTransform()),
                        rotation(btQuaternion()),

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
                        rightMouse(0),
                        lastTicks(0),
                        deltaTime(0)
                        { };

                gamestate_enum gamestate;
                TTF_Font *font;
                BlConfig *blConfig;

                float phi;
                float theta;

                btVector3 position;
                btVector3 direction;
                btTransform view;
                btQuaternion rotation;
                btVector3 rightDirection;
                btVector3 upDirection;

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

                Uint32 lastTicks;
                float deltaTime;

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
                void refreshDeltaTime();

                void computeDirection();
                void computeView();
                btVector3 getDeltaPosition();

        private:
                void computeNewAngles();
                void incrementAxis(SDL_Keymod mod, int *normalAxis,
                                int *modAxis);
};

#endif
