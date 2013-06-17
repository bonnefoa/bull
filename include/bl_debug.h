#ifndef BL_DEBUG_H
#define BL_DEBUG_H

#include <bl_config.h>
#include <bl_debug_drawer.h>
#include <bl_text.h>
#include <bl_state.h>
#include <bl_simulation.h>

class BlDebug
{
        public:
                BlDebug(BlConfig *_blConfig,
                                BlState *_blState,
                                BlDebugDrawer *_blDebugDrawer,
                                BlSimulation *_blSimulation,
                                BlText *_blText) :
                        blConfig(_blConfig),
                        blState(_blState),
                        blDebugDrawer(_blDebugDrawer),
                        blSimulation(_blSimulation),
                        blText(_blText) {};
                ~BlDebug();

                void printFps();
                void renderDebug();

        private:
                BlConfig *blConfig;
                BlState *blState;
                BlDebugDrawer *blDebugDrawer;
                BlSimulation *blSimulation;
                BlText *blText;

                int countFrame;
                float currentDelta;
                float currentFps;
};

#endif
