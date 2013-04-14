#ifndef BL_SDL_H
#define BL_SDL_H

#include <SDL.h>

class BlSdl {
        public:
                BlSdl(void);
                void launch(void);
                void shutdown(void);

        private:
                SDL_Window *main_window;
                SDL_GLContext main_context;
                void die(const char *msg);
                void check_error(int line = -1);
};

#endif
