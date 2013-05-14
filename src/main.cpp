#include <bl_input.h>
#include <bl_window.h>
#include <bl_simulation.h>
#include <bl_program_model.h>
#include <bl_program_shadow.h>
#include <bl_loader.h>
#include <bl_scene.h>
#include <bl_log.h>

#define TICK_INTERVAL 1000

BlInput *blInput;
BlWindow *blWindow;
BlSimulation *blSimulation;
BlProgramModel *blProgramModel;
BlProgramShadow *blProgramShadow;
BlScene *blScene;
Uint32 nextTime = 0;

void init()
{
        blWindow = new BlWindow();
        blWindow->launch();
        blInput = new BlInput();
        blSimulation = new BlSimulation();
}

void clean()
{
        if(blProgramModel) {
                delete blProgramModel;
        }
        if(blScene) {
                delete blScene;
        }
        blSimulation->clearWorld();
}

void shutdown()
{
        clean();
        blWindow->shutdown();
        delete blWindow;
        delete blInput;
        delete blSimulation;
}

void initPrograms()
{
        blProgramModel = getProgramModel(blInput);
        blProgramShadow = getProgramShadow();
}

void initScene(char *filename)
{
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
                light->init();
                light->loadInBuffer(blProgramModel->programId);
        }
        blScene->blLightAmbient->loadInBuffer(blProgramModel->programId);
}

void logState()
{
        if(nextTime <= blInput->now) {
                nextTime = blInput->now + TICK_INTERVAL;
                blInput->logState();
        }
}

void renderLight() {
        BlLightPoint *light = blScene->blLightPoints->at(0);
        if(blInput->lDown > 0 || blInput->rightMouse > 0) {
                light->moveLight(blInput->position,
                                blProgramModel->programId);
                blProgramShadow->moveLight(blInput->position);
        }
        for (std::vector<BlModel*>::iterator
                        it = light->blModels->begin();
                        it != light->blModels->end(); ++it) {
                blProgramModel->displayModel(*it);
        }
}

void render()
{
        blProgramShadow->displaySceneForRender(blScene);
        for (std::vector<BlModel*>::iterator it = blScene->blModels->begin();
                        it != blScene->blModels->end(); ++it) {
                blProgramModel->displayModel(*it);
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

        glUseProgram(blProgramModel->programId);
        renderLight();
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
                                clean();
                                initPrograms();
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
        shutdown();
        return 0;
}
