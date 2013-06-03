#include "bl_state.h"
#include <bl_util.h>
#include <bl_log.h>
#include <bl_matrix.h>

void BlState::incrementAxis(SDL_Keymod mod, int *normalAxis, int *modAxis)
{
        if(mod == KMOD_LSHIFT) {
                *modAxis = min(*modAxis+1, MAX_AXIS);
        } else {
                *normalAxis = min(*normalAxis+1, MAX_AXIS);
        }
}

void BlState::forward(SDL_Keymod mod)
{
        incrementAxis(mod, &axisUp, &sAxisUp);
}

void BlState::reload(SDL_Keymod mod)
{
        if(mod == KMOD_LSHIFT) {
                gamestate = RELOAD_KEEP_STATE;
        } else {
                gamestate = RELOAD;
        }
}

void BlState::back(SDL_Keymod mod)
{
        incrementAxis(mod, &axisDown, &sAxisDown);
}

void BlState::left(SDL_Keymod mod)
{
        incrementAxis(mod, &axisLeft, &sAxisLeft);
}

void BlState::right(SDL_Keymod mod)
{
        incrementAxis(mod, &axisRight, &sAxisRight);
}

void BlState::stopForward()
{
        sAxisUp = 0;
        axisUp = 0;
}

void BlState::stopBack()
{
        axisDown = 0;
        sAxisDown = 0;
}

void BlState::stopLeft()
{
        axisLeft = 0;
        sAxisLeft = 0;
}

void BlState::stopRight()
{
        axisRight = 0;
        sAxisRight = 0;
}

void BlState::light()
{
        lightState = 1;
}

void BlState::stopLight()
{
        lightState = 0;
}

void BlState::debug()
{
        if(debugState == 0) {
                debugState = 1;
        } else {
                debugState = 0;
        };
}

void BlState::pause()
{
        if(gamestate == STOP) {
                gamestate = NORMAL;
        } else {
                gamestate = STOP;
        };
}


void BlState::logState()
{
        INFO("position %f %f %f\n", position[0], position[1], position[2]);
        INFO("V\n");
        printBtTransform(&view);
}
