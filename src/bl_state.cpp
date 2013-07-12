#include "bl_state.h"
#include <bl_util.h>
#include <bl_log.h>
#include <bl_matrix.h>

float BlState::getDeltaY()
{
        return float(axisUp - axisDown);
}

float BlState::getDeltaX()
{
        return float(axisRight - axisLeft);
}

float BlState::getDeltaRotY()
{
        return float(-mouseDeltaY + sAxisUp - sAxisDown);
}

float BlState::getDeltaRotX()
{
        return float(mouseDeltaX + sAxisRight - sAxisLeft);
}

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

void BlState::refreshState()
{
        Uint32 now = SDL_GetTicks();
        deltaTime = float(now - lastTicks);
        lastTicks = now;

        SDL_GetRelativeMouseState(&mouseDeltaX, &mouseDeltaY);
}
