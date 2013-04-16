#include <bl_input.h>
#include <SDL.h>

BlInput::BlInput()
{
        for(int i = 0; i < 322; i++) {
                keys[i] = false;
        }
}

void BlInput::pollInput()
{
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
                switch (event.type) {
                        case SDL_QUIT:
                                gameState = 1;
                                break;
                        case SDL_KEYDOWN:
                                keys[event.key.keysym.sym] = true;
                                break;
                        case SDL_KEYUP:
                                keys[event.key.keysym.sym] = false;
                                break;
                        default:
                                break;
                }
        }
}

void BlInput::handleInput()
{
        if(keys[SDLK_ESCAPE]) {
                gameState = 1;
        }
}
