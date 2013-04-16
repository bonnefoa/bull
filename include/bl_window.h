#ifndef BL_SDL_H
#define BL_SDL_H

#include <SDL.h>

class BlWindow {
        public:
                SDL_Window *window;
                SDL_GLContext context;

                BlWindow(void);
                void launch(void);
                void shutdown(void);

        private:
                void die(const char *msg);
                void checkError(int line = -1);
};

#endif
