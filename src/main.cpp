#include <bl_state.h>
#include <bl_input.h>
#include <bl_loader.h>
#include <bl_log.h>
#include <bl_program_model.h>
#include <bl_program_shadow.h>
#include <bl_program_texture.h>
#include <bl_scene.h>
#include <bl_simulation.h>
#include <bl_window.h>

#define TICK_INTERVAL 1000

BlInput *blInput;
BlWindow *blWindow;

BlSimulation *blSimulation;
BlProgramModel *blProgramModel;
BlProgramShadow *blProgramShadow;
BlProgramTexture *blProgramTexture;

BlScene *blScene;
BlState *blState;
BlConfig *blConfig;
Uint32 nextTime = 0;

void initWindow()
{
        blWindow = new BlWindow();
        blWindow->launch();
}

void initBullora()
{
        blConfig = loadBlConfig("conf.ini");

        blState = new BlState(btVector3(0,0,8));
        blInput = new BlInput(blState, blConfig);

        blSimulation = new BlSimulation(blConfig, blState);
        blProgramModel = getProgramModel(blInput, blConfig, blState);
        blProgramTexture = getProgramTexture();
        blProgramShadow = getProgramShadow(btVector3());
}

void clean()
{
        delete blProgramModel;
        delete blProgramShadow;
        delete blProgramTexture;
        delete blScene;
        delete blSimulation;
        delete blInput;
        delete blConfig;
}

void shutdown()
{
        clean();
        blWindow->shutdown();
        delete blWindow;
}

void initScene(char *filename)
{
        blScene = loadXmlScene(filename);
        blProgramModel->bindProjection();
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
                blState->logState();
        }
}

void setLight() {
        BlLightPoint *light = blScene->blLightPoints->at(0);
        blProgramShadow->moveLight(blState->position);
        blProgramModel->moveLight(blState->position);
        light->moveLight(blState->position,
                        blProgramModel->programId);
}

void moveLight() {
        if(blState->lightState > 0) {
                setLight();
        }
}

void render()
{
        glViewport(0, 0, 1024, 1024);
        blProgramShadow->displaySceneForRender(blScene);
        blProgramModel->displayScene(blScene, blProgramShadow->depthTexture);
        blSimulation->debugDraw();
        SDL_GL_SwapWindow(blWindow->window);
}

void mainLoop()
{
        glClearColor( 0.0, 0.0, 0.2, 1.0 );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        blInput->handleInput();
        blInput->handleMovement();
        logState();

        moveLight();
        render();
}

int main(int argc, char **argv)
{
        (void) argc;
        (void) argv;

        initWindow();
        initBullora();
        initScene(argv[1]);
        setLight();

        while(true) {
                mainLoop();
                switch (blState->gamestate) {
                        case QUIT:
                                blWindow->shutdown();
                                return 0;
                        case RELOAD:
                                clean();
                                initBullora();
                                initScene(argv[1]);
                                blState->gamestate = NORMAL;
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
