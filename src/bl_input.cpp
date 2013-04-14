#include <bl_input.h>
#include <SDL.h>

BlInput::BlInput()
{
        for(int i = 0; i < 322; i++) {
                keys[i] = false;
        }
}

void BlInput::poll_input()
{
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
                switch (event.type) {
                        case SDL_QUIT:
                                game_state = 0;
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

void BlInput::handle_input()
{
        if(keys[SDLK_ESCAPE]) {
                game_state = 0;
        }
}
