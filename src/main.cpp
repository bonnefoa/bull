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
#include <bl_camera.h>
#include <bl_text.h>
#include <bl_debug.h>
#include <bl_program_skybox.h>
#include <bl_skybox.h>

BlInput *blInput;
BlSdl *blSdl;

BlSimulation *blSimulation;
BlProgramModel *blProgramModel;
BlProgramTerrain *blProgramTerrain;
BlProgramShadow *blProgramShadow;
BlProgramTexture *blProgramTexture;
BlProgramSkybox *blProgramSkybox;

BlTexture *blTexture;
BlLoader *blLoader;

BlProgramDebug *blProgramDebug;

BlProgramText2d *blProgramText2d;
BlProgramText3d *blProgramText3d;
BlText *blText;

BlScene *blScene;
BlState *blState;
BlConfig *blConfig;
BlCamera *blCamera;

BlDebugDrawer *blDebugDrawer;
BlDebug *blDebug;

void initWindow()
{
        blSdl = new BlSdl(blConfig);
        blSdl->launch();
        blTexture = new BlTexture();
}

void initComponents(const char *filename)
{
        blState = new BlState(blSdl->font, blConfig);
        blCamera = new BlCamera(blConfig, blState);
        blInput = new BlInput(blState, blConfig, blCamera);

        blProgramModel = getProgramModel(blConfig, blState, blCamera);
        blProgramTerrain = getProgramTerrain(blConfig, blCamera);
        blProgramTexture = getProgramTexture();
        blProgramSkybox = getProgramSkybox(blConfig, blCamera);
        blProgramShadow = getProgramShadow(btVector3());
        blLoader = new BlLoader(blTexture, blState);

        blProgramText2d = getProgramText2d(blConfig);
        blProgramText3d = getProgramText3d(blConfig);
        blText = new BlText(blProgramText2d, blProgramText3d, blConfig, blState);
        blText->init();

        blProgramDebug = getProgramDebug(blConfig);
        blDebugDrawer = new BlDebugDrawer(blProgramDebug, blState,
                        blCamera, blText);
        blDebugDrawer->init();
        blSimulation = new BlSimulation(blDebugDrawer, blState);

        blScene = blLoader->loadScene(filename);
        blProgramModel->bindProjection();
        blProgramTerrain->bindProjection();
        blProgramSkybox->bindProjection();
        blScene->init(blSimulation, blProgramModel->programId);

        blDebug = new BlDebug(blConfig, blState,
                        blDebugDrawer, blSimulation, blText, blScene);
}

void clean()
{
        delete blProgramModel;
        delete blProgramShadow;
        delete blProgramTexture;
        delete blProgramTerrain;
        delete blProgramText2d;

        delete blScene;
        delete blSimulation;
        delete blInput;
        delete blText;
}

void shutdown()
{
        clean();
        blSdl->shutdown();
        delete blSdl;
        delete blConfig;
        delete blTexture;
}

void setLight() {
        BlLightPoint *light = blScene->blLightPoints->at(0);
        blProgramShadow->moveLight(blScene->blCharacter->getPosition() + btVector3(0, 5, 0));
        blProgramModel->moveLight(blScene->blCharacter->getPosition() + btVector3(0, 5, 0));
        light->moveLight(blScene->blCharacter->getPosition() + btVector3(0, 5, 0));
        light->loadLightInProgram(blProgramModel->programId);
        light->loadLightInProgram(blProgramTerrain->programId);
}

void moveLight() {
        if(blState->lightState > 0) {
                setLight();
        }
}

void render()
{
        glViewport(0, 0, blConfig->width, blConfig->height);
        blProgramShadow->displaySceneForRender(blScene);
        blProgramModel->displayScene(blScene, blProgramShadow->depthTexture);
        blProgramTerrain->displayScene(blScene);
        blProgramSkybox->displayScene(blScene);
        blDebug->renderDebug();

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
        initComponents(configFile);
        setLight();
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
        initComponents(configFile);
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
