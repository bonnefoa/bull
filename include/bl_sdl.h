#ifndef BL_SDL_H
#define BL_SDL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <bl_config.h>

class BlSdl {
        public:
                BlSdl(BlConfig *_blConfig) : blConfig(_blConfig) {};
                SDL_Window *window;
                SDL_GLContext context;

                BlSdl(void);
                void launch(void);
                void shutdown(void);
                TTF_Font *font;

        private:
                void die(const char *msg);
                void checkError(int line = -1);
                BlConfig *blConfig;
                void initFont();
};

#endif
