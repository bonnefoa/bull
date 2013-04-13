#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <SDL.h>

void sdl_die(const char *msg)
{
        printf("%s: %s\n", msg, SDL_GetError());
        SDL_Quit();
        exit(1);
}

void sdl_check_error(int line = -1)
{
#ifndef NDEBUG
        const char *error = SDL_GetError();
        if (*error != '\0')
        {
                printf("SDL Error: %s\n", error);
                if (line != -1)
                        printf(" + line: %i\n", line);
                SDL_ClearError();
        }
#endif
}

void sdl_launch()
{
        SDL_Window *main_window;
        SDL_GLContext main_context;

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
                sdl_die("Unable to initialize SDL");
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

        main_window = SDL_CreateWindow("bullora"
                        , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (!main_window)
                sdl_die("Unable to create window");
        sdl_check_error(__LINE__);

        main_context = SDL_GL_CreateContext(main_window);
        sdl_check_error(__LINE__);

        SDL_GL_SetSwapInterval(1);

        glClearColor ( 1.0, 0.0, 0.0, 1.0 );
        glClear ( GL_COLOR_BUFFER_BIT );
        SDL_GL_SwapWindow(main_window);
        SDL_Delay(2000);

        SDL_GL_DeleteContext(main_context);
        SDL_DestroyWindow(main_window);
        SDL_Quit();
}
