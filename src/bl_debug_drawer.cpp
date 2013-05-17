#include "bl_debug_drawer.h"

void BlDebugDrawer::init()
{
        glGenBuffers(1, &lineBuffer);
        glGenBuffers(1, &colorBuffer);
}

void BlDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
        colors.clear();
        colors.push_back(color);
        colors.push_back(color);

        lines.clear();
        lines.push_back(from);
        lines.push_back(to);

        glBindBuffer(GL_COLOR_ARRAY, colorBuffer);
        glBufferData(GL_COLOR_ARRAY, 6 * sizeof(GL_FLOAT), &colors[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GL_FLOAT), &lines[0], GL_STATIC_DRAW);

        glDrawArrays(GL_LINE, 0, 2);
}
