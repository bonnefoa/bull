#include "bl_debug.h"

void BlDebug::printFps()
{
        if(countFrame % 60 == 0) {
                currentDelta = blState->deltaTime;
                currentFps = 1000.0f / blState->deltaTime;
                countFrame = 0;
        }
        countFrame++;
        char str[250];
        sprintf(str, "%.2f ms, %.2f fps", currentDelta, currentFps);
        blText->print2dText(str, 0, blConfig->height * 3.0f / 4.0f );
}

void BlDebug::renderDebug()
{
        if(blState->debugState) {
                blDebugDrawer->initDebugRender();
                blSimulation->debugDraw();
                blDebugDrawer->finalizeDraw();
                printFps();
        }
}
