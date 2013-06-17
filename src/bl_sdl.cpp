#include <GL/glew.h>
#include <bl_sdl.h>
#include <stdio.h>
#include <stdlib.h>
#include <bl_log.h>

void errorCallback( GLenum source,
                GLenum type, GLuint id, GLenum severity,
                GLsizei length, const char* message,
                void* userParam)
{
        (void) userParam;
        (void) source;
        (void) type;
        (void) id;
        (void) length;
        (void) severity;
        INFO("Source %x, type %x, id %i\n", source, type, id);
        INFO("%s\n",message);
}

void BlSdl::die(const char *msg)
{
        INFO("%s: %s\n", msg, SDL_GetError());
        SDL_Quit();
        exit(1);
}

void BlSdl::checkError(int line)
{
        const char *error = SDL_GetError();
        if (*error != '\0')
        {
                INFO("SDL Error: %s, line %i\n", error, line);
                SDL_ClearError();
        }
}

void BlSdl::shutdown()
{
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
}

void BlSdl::initFont()
{
        if(TTF_Init() < 0) {
                ERROR("TTF_Init: %s\n", TTF_GetError());
        }
        font = TTF_OpenFont(blConfig->fontPath, blConfig->fontSize);
        if(!font) {
                ERROR("TTF_OpenFont: font '%s' %s\n",
                                blConfig->fontPath,
                                TTF_GetError());
        }
}

void BlSdl::launch()
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
                        blConfig->width, blConfig->height
                        , SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_GRABBED);
        if (!window)
                die("Unable to create window");
        checkError(__LINE__);

        context = SDL_GL_CreateContext(window);
        checkError(__LINE__);
        initFont();

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (glewInit() != GLEW_OK)
                die("Failed to initialize GLEW\n");
        glDebugMessageCallbackARB( errorCallback, NULL );

        SDL_GL_SetSwapInterval(1);
}
