#ifndef BL_DEBUG_H
#define BL_DEBUG_H

#include <bl_config.h>
#include <bl_debug_drawer.h>
#include <bl_text.h>
#include <bl_state.h>
#include <bl_simulation.h>
#include <bl_scene.h>

class BlDebug
{
        public:
                BlDebug(BlConfig *_blConfig,
                                BlState *_blState,
                                BlDebugDrawer *_blDebugDrawer,
                                BlSimulation *_blSimulation,
                                BlText *_blText,
                                BlScene *_blScene) :
                        blConfig(_blConfig),
                        blState(_blState),
                        blDebugDrawer(_blDebugDrawer),
                        blSimulation(_blSimulation),
                        blText(_blText),
                        blScene(_blScene) {};
                ~BlDebug();

                void printFps();
                void renderDebug(btTransform view);

        private:
                BlConfig *blConfig;
                BlState *blState;
                BlDebugDrawer *blDebugDrawer;
                BlSimulation *blSimulation;
                BlText *blText;
                BlScene *blScene;

                int countFrame;
                float currentDelta;
                float currentFps;

                void debugTerrain(BlTerrain *terrain);
                void debugModel(BlModel *model);
};

#endif
