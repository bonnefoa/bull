#include <bl_input.h>
#include <bl_util.h>
#include <math.h>
#include <bl_log.h>
#include <bl_matrix.h>

BlInput::BlInput(BlState *state, BlConfig *config)
{
        blState = state;
        blConfig = config;

        SDL_SetRelativeMouseMode(SDL_TRUE);
}

void BlInput::handleMouseUp(SDL_Event *event)
{
        switch(event->button.button) {
                case SDL_BUTTON_LEFT:
                        blState->leftMouse = 0;
                        break;
                case SDL_BUTTON_RIGHT:
                        blState->rightMouse = 0;
                        break;
        }
}

void BlInput::handleMouseDown(SDL_Event *event)
{
        switch(event->button.button) {
                case SDL_BUTTON_LEFT:
                        blState->leftMouse = 1;
                        break;
                case SDL_BUTTON_RIGHT:
                        blState->rightMouse = 1;
                        break;
        }
}

void BlInput::handleKeyUp(SDL_Event *event)
{
        int sym = event->key.keysym.sym;
        if(sym == blConfig->key_forward) {
                blState->stopForward();
        } else if(sym == blConfig->key_back) {
                blState->stopBack();
        } else if(sym == blConfig->key_left) {
                blState->stopLeft();
        } else if(sym == blConfig->key_right) {
                blState->stopRight();
        } else if(sym == blConfig->key_light) {
                blState->stopLight();
        }
}

void BlInput::handleKeyDown(SDL_Event *event)
{
        SDL_Keymod mod = SDL_GetModState();
        int sym = event->key.keysym.sym;
        if(sym == blConfig->key_forward) {
                blState->forward(mod);
        } else if(sym == blConfig->key_back) {
                blState->back(mod);
        } else if(sym == blConfig->key_left) {
                blState->left(mod);
        } else if(sym == blConfig->key_right) {
                blState->right(mod);
        } else if(sym == blConfig->key_escape) {
                blState->gamestate = QUIT;
        } else if(sym == blConfig->key_alt_escape) {
                blState->gamestate = QUIT;
        } else if(sym == blConfig->key_reload) {
                blState->reload(mod);
        } else if(sym == blConfig->key_light) {
                blState->light();
        } else if(sym == blConfig->key_pause) {
                blState->pause();
        } else if(sym == blConfig->key_debug) {
                blState->debug();
        }
}

void BlInput::handleInput()
{
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
                switch (event.type) {
                        case SDL_QUIT:
                                blState->gamestate = QUIT;
                                break;
                        case SDL_KEYDOWN:
                                handleKeyDown(&event);
                                break;
                        case SDL_KEYUP:
                                handleKeyUp(&event);
                                break;
                        case SDL_MOUSEBUTTONDOWN:
                                handleMouseDown(&event);
                                break;
                        case SDL_MOUSEBUTTONUP:
                                handleMouseUp(&event);
                                break;
                        default:
                                break;
                }
        }
}
