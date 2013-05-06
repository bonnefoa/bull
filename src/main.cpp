#include <bl_input.h>
#include <bl_window.h>
#include <bl_simulation.h>
#include <bl_program_model.h>
#include <bl_loader.h>
#include <bl_log.h>

#define TICK_INTERVAL 1000

BlInput *blInput;
BlWindow *blWindow;
BlSimulation *blSimulation;
BlProgramModel *blProgramModel;
std::vector<BlModel*> *blModels;
Uint32 nextTime = 0;

void init()
{
        blWindow = new BlWindow();
        blWindow->launch();
        blInput = new BlInput();
        blSimulation = new BlSimulation();
}

void initPrograms()
{
        std::vector<BlShader*> shaders;
        BlShader *modelVertexShader = new BlShader("glsl/model_vertex.glsl"
                        , GL_VERTEX_SHADER);
        BlShader *modelFragmentShader = new BlShader("glsl/model_fragment.glsl"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(modelVertexShader);
        shaders.push_back(modelFragmentShader);

        blProgramModel = new BlProgramModel(shaders, blInput);
        blProgramModel->loadProgram();
        blProgramModel->init();
}

void initModels(char *filename)
{
        blModels = loadScene(filename);
        for (std::vector<BlModel*>::iterator it = blModels->begin();
                        it != blModels->end(); ++it) {
                INFO("Load in buffer\n");
                (*it)->init();
                blProgramModel->loadModelInBuffer(*it);
        }
}

void logState()
{
        if(nextTime <= blInput->now) {
                nextTime = blInput->now + TICK_INTERVAL;
                blInput->logState();
        }
}

void main_loop()
{
        glClearColor( 0.0, 0.0, 0.2, 1.0 );
        glClear( GL_COLOR_BUFFER_BIT );
        blInput->handleInput();
        logState();

        for (std::vector<BlModel*>::iterator it = blModels->begin();
                        it != blModels->end(); ++it) {
                blProgramModel->displayModel(*it);
        }
}

int main(int argc, char **argv)
{
        (void) argc;
        (void) argv;

        init();
        initPrograms();
        initModels(argv[1]);
        while(true) {
                main_loop();
                if(blInput->gameState == 1) {
                        blWindow->shutdown();
                        return 0;
                }
                SDL_GL_SwapWindow(blWindow->window);
        }
        return 1;
}
