#include <bl_input.h>
#include <bl_window.h>
#include <bl_simulation.h>
#include <bl_program_model.h>
#include <bl_loader.h>
#include <bl_scene.h>
#include <bl_log.h>

#define TICK_INTERVAL 1000

BlInput *blInput;
BlWindow *blWindow;
BlSimulation *blSimulation;
BlProgramModel *blProgramModel;
BlScene *blScene;
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

void initScene(char *filename)
{
        if(blScene) {
                delete blScene;
        }
        blSimulation->clearWorld();

        blScene = loadXmlScene(filename);
        blProgramModel->bindProjectionMatrix();
        for (std::vector<BlModel*>::iterator it = blScene->blModels->begin();
                        it != blScene->blModels->end(); ++it) {
                BlModel *model = *it;
                model->init();
                model->loadInBuffer();
                model->rigidBody = blSimulation->addBlModel(model);
        }
        for (std::vector<BlLightPoint*>::iterator it = blScene->blLightPoints->begin();
                        it != blScene->blLightPoints->end(); ++it) {
                BlLightPoint *light = *it;
                light->loadInBuffer(blProgramModel->uniformLightPosition,
                                blProgramModel->uniformLightColor);
        }
        blScene->blLightAmbient->loadInBuffer(blProgramModel->uniformAmbientColor);

}

void logState()
{
        if(nextTime <= blInput->now) {
                nextTime = blInput->now + TICK_INTERVAL;
                blInput->logState();
        }
}

void render()
{
        for (std::vector<BlModel*>::iterator it = blScene->blModels->begin();
                        it != blScene->blModels->end(); ++it) {
                blProgramModel->displayModel(*it);
        }
        if(blInput->lDown > 0) {
                BlLightPoint *light = blScene->blLightPoints->at(0);
                light->position = blInput->position;
                light->loadInBuffer(blProgramModel->uniformLightPosition,
                                blProgramModel->uniformLightColor);
        }
        SDL_GL_SwapWindow(blWindow->window);
}

void mainLoop()
{
        glClearColor( 0.0, 0.0, 0.2, 1.0 );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        blInput->handleInput();
        blInput->handleMovement();
        logState();
        render();
}

int main(int argc, char **argv)
{
        (void) argc;
        (void) argv;

        init();
        initPrograms();
        initScene(argv[1]);
        while(true) {
                mainLoop();
                switch (blInput->state) {
                        case QUIT:
                                blWindow->shutdown();
                                return 0;
                        case RELOAD:
                                initScene(argv[1]);
                                blInput->state = NORMAL;
                                break;
                        case NORMAL:
                                blSimulation->step();
                                break;
                        case STOP:
                                break;
                }
        }
        return 1;
}
