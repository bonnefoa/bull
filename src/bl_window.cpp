#include <GL/glew.h>
#include <bl_window.h>
#include <stdio.h>
#include <stdlib.h>

void errorCallback( GLenum source,
                GLenum type, GLuint id, GLenum severity,
                GLsizei length, const char* message,
                void* userParam)
{
        (void) userParam;
        printf("%s\t",message);
        printf("%d, %d, %d, %d, %d\n", source, type
                        , id, length, severity);
}

BlWindow::BlWindow()
{
}

void BlWindow::die(const char *msg)
{
        printf("%s: %s\n", msg, SDL_GetError());
        SDL_Quit();
        exit(1);
}

void BlWindow::checkError(int line)
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

void BlWindow::shutdown()
{
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
}

void BlWindow::launch()
{

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
                die("Unable to initialize SDL");
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

        window = SDL_CreateWindow("bullora"
                        , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (!window)
                die("Unable to create window");
        checkError(__LINE__);

        context = SDL_GL_CreateContext(window);
        checkError(__LINE__);

        if (glewInit() != GLEW_OK)
                die("Failed to initialize GLEW\n");
        glDebugMessageCallbackARB( errorCallback, NULL );

        SDL_GL_SetSwapInterval(1);
}
