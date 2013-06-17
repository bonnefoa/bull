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
#include <bl_matrix.h>
#include <bl_network.h>
#include <bl_camera.h>

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
BlNetwork *blNetwork;

BlProgramDebug *blProgramDebug;
BlDebugDrawer *blDebugDrawer;

BlScene *blScene;
BlState *blState;
BlConfig *blConfig;
BlCamera *blCamera;
Uint32 nextTime = 0;

void initWindow()
{
        blSdl = new BlSdl(blConfig);
        blSdl->launch();
        blTexture = new BlTexture();
}

void initComponents()
{
        blState = new BlState(blSdl->font, blConfig);
        blCamera = new BlCamera(blConfig, blState);
        blInput = new BlInput(blState, blConfig, blCamera);

        blProgramModel = getProgramModel(blConfig, blState, blCamera);
        blProgramTerrain = getProgramTerrain(blConfig, blCamera);
        blProgramTexture = getProgramTexture();
        blProgramShadow = getProgramShadow(btVector3());
        blLoader = new BlLoader(blTexture, blState);

        blProgramDebug = getProgramDebug(blConfig);
        blDebugDrawer = new BlDebugDrawer(blProgramDebug, blState,
                        blCamera);
        blDebugDrawer->init();
        blSimulation = new BlSimulation(blDebugDrawer, blState);
        blNetwork = new BlNetwork(blConfig);
        blNetwork->init();
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
}

void shutdown()
{
        clean();
        blSdl->shutdown();
        delete blSdl;
        delete blConfig;
        delete blTexture;
}

void initScene(const char *filename)
{
        blScene = blLoader->loadScene(filename);
        blProgramModel->bindProjection();
        blProgramTerrain->bindProjection();
        blScene->init(blSimulation, blProgramModel->programId);
}

void setLight() {
        BlLightPoint *light = blScene->blLightPoints->at(0);
        blProgramShadow->moveLight(blScene->blCharacter->getPosition());
        blProgramModel->moveLight(blScene->blCharacter->getPosition());
        light->moveLight(blScene->blCharacter->getPosition(),
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
                blDebugDrawer->drawXYZAxis(buildModelMatrix(btVector3(1,1,1),
                                        terrain->position));
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
        glViewport(0, 0, blConfig->width, blConfig->height);
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

        blState->refreshState();
        blInput->handleInput();

        blCamera->moveCamera(blScene->blCharacter->getPosition());
        blCamera->computeNewCamera();
        blScene->blCharacter->handleMovement();
        blScene->blCharacter->handleTurn(blCamera->rotation);

        if(blState->leftMouse == 1) {
                blSimulation->pushObject(blScene->blCharacter);
        }
        if(blState->rightMouse == 1) {
                blSimulation->pickObject(blScene->blCharacter);
        } else {
                blSimulation->endPickObject(blScene->blCharacter);
        }

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
        btVector3 oldPosition = blScene->blCharacter->getPosition();
        reload(configFile);
        blScene->blCharacter->getPosition() = oldPosition;
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
