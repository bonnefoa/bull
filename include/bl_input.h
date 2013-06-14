#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

#include <bullet.h>
#include <SDL.h>
#include <bl_state.h>
#include <bl_config.h>
#include <bl_camera.h>

class BlInput
{

        public:
                BlInput(BlState *blState, BlConfig *config,
                                BlCamera *blCamera);
                void handleInput();

        private:
                void handleKeyDown(SDL_Event *event);
                void handleKeyUp(SDL_Event *event);
                void handleMouseDown(SDL_Event *event);
                void handleMouseUp(SDL_Event *event);

                btVector3 right;
                btVector3 up;

                BlState *blState;
                BlConfig *blConfig;
                BlCamera *blCamera;
};

#endif
