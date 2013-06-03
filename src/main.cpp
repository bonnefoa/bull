#include <bl_state.h>
#include <bl_input.h>
#include <bl_loader.h>
#include <bl_log.h>
#include <bl_program_model.h>
#include <bl_program_shadow.h>
#include <bl_program_texture.h>
#include <bl_program_terrain.h>
#include <bl_scene.h>
#include <bl_simulation.h>
#include <bl_sdl.h>

#define TICK_INTERVAL 1000

BlInput *blInput;
BlSdl *blSdl;

BlSimulation *blSimulation;
BlProgramModel *blProgramModel;
BlProgramTerrain *blProgramTerrain;
BlProgramShadow *blProgramShadow;
BlProgramTexture *blProgramTexture;
BlTexture *blTexture;
BlLoader *blLoader;

BlProgramDebug *blProgramDebug;
BlDebugDrawer *blDebugDrawer;

BlScene *blScene;
BlState *blState;
BlConfig *blConfig;
Uint32 nextTime = 0;

void initWindow()
{
        blSdl = new BlSdl(blConfig);
        blSdl->launch();
        blTexture = new BlTexture();
}

void initComponents()
{
        blState = new BlState(btVector3(0,0,8), blSdl->font);
        blInput = new BlInput(blState, blConfig);

        blProgramModel = getProgramModel(blInput, blConfig, blState);
        blProgramTerrain = getProgramTerrain(blConfig, blState);
        blProgramTexture = getProgramTexture();
        blProgramShadow = getProgramShadow(btVector3());
        blLoader = new BlLoader(blTexture);

        blProgramDebug = getProgramDebug(blConfig);
        blDebugDrawer = new BlDebugDrawer(blProgramDebug, blState);
        blDebugDrawer->init();
        blSimulation = new BlSimulation(blDebugDrawer);
}

void clean()
{
        delete blProgramModel;
        delete blProgramShadow;
        delete blProgramTexture;
        delete blProgramTerrain;
        delete blScene;
        delete blSimulation;
        delete blInput;
        delete blConfig;
}

void shutdown()
{
        clean();
        blSdl->shutdown();
        delete blSdl;
}

void initScene(const char *filename)
{
        blScene = blLoader->loadScene(filename);
        blProgramModel->bindProjection();
        blProgramTerrain->bindProjection();
        for (std::vector<BlModel*>::iterator it = blScene->blModels->begin();
                        it != blScene->blModels->end(); ++it) {
                BlModel *model = *it;
                model->init();
                model->loadInBuffer();
                blSimulation->addRigidBody(model->rigidBody);
        }
        for (std::vector<BlTerrain*>::iterator
                        it = blScene->blTerrains->begin();
                        it != blScene->blTerrains->end(); ++it) {
                BlTerrain *terrain = *it;
                terrain->init();
                terrain->loadInBuffer();
                blSimulation->addRigidBody(terrain->rigidBody);
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

void debugScene()
{
        for (std::vector<BlTerrain*>::iterator
                        it = blScene->blTerrains->begin();
                        it != blScene->blTerrains->end(); ++it) {
                BlTerrain *terrain = *it;
                blDebugDrawer->drawXYZAxis(terrain->model);
        }
}

void renderDebug()
{
        if(blState->debugState) {
                blDebugDrawer->initDebugRender();
                blSimulation->debugDraw();
                debugScene();
                blDebugDrawer->finalizeDraw();
        }
}

void render()
{
        glViewport(0, 0, 1024, 1024);
        blProgramShadow->displaySceneForRender(blScene);
        blProgramModel->displayScene(blScene, blProgramShadow->depthTexture);
        blProgramTerrain->displayScene(blScene);
        renderDebug();
        SDL_GL_SwapWindow(blSdl->window);
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

void reload(const char *configFile)
{
        clean();
        initComponents();
        initScene(configFile);
        blState->gamestate = NORMAL;
}

void reloadKeepPosition(const char *configFile)
{
        btVector3 oldPosition = blState->position;
        float oldPhi = blState->phi;
        float oldTheta = blState->theta;
        reload(configFile);
        blState->phi = oldPhi;
        blState->theta = oldTheta;
        blState->position = oldPosition;
        blSimulation->toggleDebug(blState->debugState);
}

int main(int argc, char **argv)
{
        (void) argc;
        (void) argv;

        const char *configFile = argv[1];
        blConfig = loadBlConfig("conf.yaml");

        initWindow();
        initComponents();
        initScene(configFile);
        setLight();

        while(true) {
                mainLoop();
                switch (blState->gamestate) {
                        case QUIT:
                                blSdl->shutdown();
                                return 0;
                        case RELOAD_KEEP_STATE:
                                reloadKeepPosition(configFile);
                                break;
                        case RELOAD:
                                reload(configFile);
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
