#include <bl_sdl.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

BlSdl::BlSdl()
{
}

void BlSdl::die(const char *msg)
{
        printf("%s: %s\n", msg, SDL_GetError());
        SDL_Quit();
        exit(1);
}

void BlSdl::check_error(int line)
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

void BlSdl::shutdown()
{
        SDL_GL_DeleteContext(main_context);
        SDL_DestroyWindow(main_window);
        SDL_Quit();
}

void BlSdl::launch()
{

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
                die("Unable to initialize SDL");
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

        main_window = SDL_CreateWindow("bullora"
                        , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (!main_window)
                die("Unable to create window");
        check_error(__LINE__);

        main_context = SDL_GL_CreateContext(main_window);
        check_error(__LINE__);

        SDL_GL_SetSwapInterval(1);

        glClearColor ( 1.0, 0.0, 0.0, 1.0 );
        glClear ( GL_COLOR_BUFFER_BIT );
        SDL_GL_SwapWindow(main_window);
}
