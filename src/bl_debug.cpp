#include "bl_debug.h"
#include <bl_matrix.h>

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

void BlDebug::renderDebug(btTransform view, btTransform projection)
{
        if(blState->debugState) {
                blDebugDrawer->initDebugRender(view, projection);
                blSimulation->debugDraw();
                for(std::vector<BlTerrain*>::iterator it = blScene->blTerrains->begin();
                                it != blScene->blTerrains->end();
                                it++) {
                        debugTerrain(*it);
                }
                for(std::vector<BlModel*>::iterator it = blScene->blModels->begin();
                                it != blScene->blModels->end();
                                it++) {
                        debugModel(*it);
                }
                blDebugDrawer->finalizeDraw();
                printFps();
        }
}

void BlDebug::debugModel(BlModel *model)
{
        btTransform trans = model->getModelTansform();
        btMatrix3x3 ident;
        ident.setIdentity();
        for(unsigned int i = 0; i < model->vertices.size(); i++) {
                btVector3 orig = trans * model->vertices.at(i);
                btTransform center = btTransform(ident, orig);
                btVector3 normal = trans.getBasis() * model->normals.at(i);
                btVector3 tangent = trans.getBasis() * model->tangents.at(i);
                btVector3 bitangent = trans.getBasis() * model->bitangents.at(i);
                blDebugDrawer->drawAxis(center, normal,
                        btVector3(0.5, 1, 0.5));
                blDebugDrawer->drawAxis(center, tangent,
                        btVector3(1,0.5,0.5));
                blDebugDrawer->drawAxis(center, bitangent,
                        btVector3(0.5,0.5,1));
        }
}

void BlDebug::debugTerrain(BlTerrain *terrain)
{
        btTransform model = buildModelMatrix(terrain->scale, terrain->position);
        btMatrix3x3 ident;
        ident.setIdentity();
        for(unsigned int i = 0; i < terrain->vertices.size(); i++) {
                btVector3 pos = model * terrain->vertices.at(i);
                btTransform center = btTransform(ident, pos);
                btVector3 normal = terrain->normals.at(i);
                btVector3 tangent = terrain->tangents.at(i);
                btVector3 bitangent = terrain->bitangents.at(i);
                blDebugDrawer->drawAxis(center, normal, btVector3(0,1,0));
                blDebugDrawer->drawAxis(center, tangent, btVector3(1,0,0));
                blDebugDrawer->drawAxis(center, bitangent, btVector3(0,0,1));
        }
}
