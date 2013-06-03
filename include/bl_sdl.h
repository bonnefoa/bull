#ifndef BL_SDL_H
#define BL_SDL_H

#include <SDL.h>
#include <SDL_ttf.h>

class BlSdl {
        public:
                SDL_Window *window;
                SDL_GLContext context;

                BlSdl(void);
                void launch(void);
                void shutdown(void);

        private:
                void die(const char *msg);
                void checkError(int line = -1);
};

#endif
