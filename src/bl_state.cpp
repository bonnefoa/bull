#include "bl_state.h"
#include <bl_util.h>

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

void BlState::pause()
{
        if(gamestate == STOP) {
                gamestate = NORMAL;
        } else {
                gamestate = STOP;
        };
}
