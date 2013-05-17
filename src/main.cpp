#include <bl_input.h>
#include <bl_window.h>
#include <bl_simulation.h>
#include <bl_program_model.h>
#include <bl_program_shadow.h>
#include <bl_program_texture.h>
#include <bl_program_debug.h>
#include <bl_loader.h>
#include <bl_scene.h>
#include <bl_log.h>
#include <bl_state.h>

#define TICK_INTERVAL 1000

BlInput *blInput;
BlWindow *blWindow;

BlSimulation *blSimulation;
BlProgramModel *blProgramModel;
BlProgramShadow *blProgramShadow;
BlProgramTexture *blProgramTexture;
BlProgramDebug *blProgramDebug;

BlScene *blScene;
BlState *blState;
BlConfig *blConfig;
Uint32 nextTime = 0;

void init()
{
        blWindow = new BlWindow();
        blWindow->launch();
        blSimulation = new BlSimulation();
        blState = new BlState(blSimulation);
        blConfig = loadBlConfig("conf.ini");
        blInput = new BlInput(blState, blConfig);
}

void clean()
{
        delete blProgramModel;
        delete blProgramShadow;
        delete blProgramTexture;
        delete blProgramDebug;
        delete blScene;
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
        blProgramModel = getProgramModel(blInput, blConfig);
        blProgramTexture = getProgramTexture();
        blProgramShadow = getProgramShadow(btVector3());
        blProgramDebug = getProgramDebug(blConfig);
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
                blInput->logState();
        }
}

void setLight() {
        BlLightPoint *light = blScene->blLightPoints->at(0);
        blProgramShadow->moveLight(blInput->position);
        blProgramModel->moveLight(blInput->position);
        light->moveLight(blInput->position,
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

        init();
        initPrograms();
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
                                initPrograms();
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
