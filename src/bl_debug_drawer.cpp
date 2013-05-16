#include "bl_debug_drawer.h"

void BlDebugDrawer::init()
{
        glGenBuffers(1, &lineBuffer);
}

void BlDebugDrawer::btIDebugDraw::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
        glDrawElements(GL_LINE, indices.size(), GL_UNSIGNED_INT, (void *)0);
}
