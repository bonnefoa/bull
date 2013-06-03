#include "bl_debug_drawer.h"
#include <bl_log.h>
#include <bl_matrix.h>

void BlDebugDrawer::init()
{
        glGenBuffers(1, &lineBuffer);
        glGenBuffers(1, &colorBuffer);
}

BlDebugDrawer::~BlDebugDrawer()
{
        glDeleteBuffers(1, &lineBuffer);
        glDeleteBuffers(1, &colorBuffer);
}

void BlDebugDrawer::setDebugMode(int debugMode)
{
        blState->debugState = debugMode;
}

int BlDebugDrawer::getDebugMode() const
{
        return blState->debugState;
}

void BlDebugDrawer::drawContactPoint(const btVector3& PointOnB,
                const btVector3& normalOnB,btScalar distance,
                int lifeTime,const btVector3& color)
{
        (void) PointOnB;
        (void) normalOnB;
        (void) distance;
        (void) lifeTime;
        (void) color;
}

void BlDebugDrawer::draw3dText(const btVector3& location,const char* textString)
{
        (void)location;
        (void)textString;
}

void BlDebugDrawer::reportErrorWarning(const char* warningString)
{
        INFO("Debug drawer warning : %s\n", warningString);
}

void BlDebugDrawer::initDebugRender()
{
        glUseProgram(blProgramDebug->programId);
        sendTransform(blState->view, blProgramDebug->locView);
        colors.clear();
        lines.clear();
        glEnableVertexAttribArray(blProgramDebug->locColor);
        glEnableVertexAttribArray(blProgramDebug->locVertices);
}

void BlDebugDrawer::finalizeDraw()
{
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(btVector3),
                        &colors[0], GL_STATIC_DRAW);
        glVertexAttribPointer(blProgramDebug->locColor, 4, GL_FLOAT
                        , GL_FALSE, 0, (void *)0);

        glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
        glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(btVector3),
                        &lines[0], GL_STATIC_DRAW);
        glVertexAttribPointer(blProgramDebug->locVertices, 4 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);

        glDrawArrays(GL_LINES, 0, lines.size());

        glDisableVertexAttribArray(blProgramDebug->locColor);
        glDisableVertexAttribArray(blProgramDebug->locVertices);
}

void BlDebugDrawer::drawAxis(const btTransform& center,
                const btVector3& direction)
{
        colors.push_back(direction);
        colors.push_back(direction);

        lines.push_back(center.getOrigin());
        lines.push_back(direction * 5);
}

void BlDebugDrawer::drawXYZAxis(const btTransform& center)
{
        drawAxis(center, btVector3(1, 0, 0));
        drawAxis(center, btVector3(0, 1, 0));
        drawAxis(center, btVector3(0, 0, 1));
}

void BlDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
        colors.push_back(color);
        colors.push_back(color);

        lines.push_back(from);
        lines.push_back(to);
}
