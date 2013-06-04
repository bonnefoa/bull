#include "bl_debug_drawer.h"
#include <bl_log.h>
#include <bl_matrix.h>
#include <bl_vertice.h>
#include <bl_image.h>
#include <bl_util.h>

void BlDebugDrawer::init()
{
        glGenBuffers(1, &lineBuffer);
        glGenBuffers(1, &colorBuffer);

        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &uvBuffer);

        glGenTextures(1, &textureBuffer);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

BlDebugDrawer::~BlDebugDrawer()
{
        glDeleteBuffers(1, &lineBuffer);
        glDeleteBuffers(1, &colorBuffer);

        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &uvBuffer);
        glDeleteTextures(1, &textureBuffer);
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

        SDL_Color textColor = {255,255,255,0};
        SDL_Surface *surface =
                TTF_RenderText_Solid(blState->font, textString, textColor);
        GLenum textureFormat = GL_RGBA;

        int width = roundUpPowerOfTwo(surface->w);
        int height = roundUpPowerOfTwo(surface->h);
        unsigned char *surfPix = (unsigned char *)surface->pixels;
        unsigned char pix[width * height * sizeof(unsigned char) * RGBA_CHANNEL];
        memset(pix, 0, width * height * sizeof(unsigned char) * RGBA_CHANNEL);
        for(int x=0; x < surface->w; x++) {
                for(int y=0; y < surface->h; y++) {
                        int indexSrc = x + y * surface->pitch;
                        int index = x * RGBA_CHANNEL + y * width * RGBA_CHANNEL;
                        int val = surfPix[indexSrc] * 255;
                        pix[index] = val;
                        pix[index+1] = val;
                        pix[index+2] = val;
                        pix[index+3] = val;
                }
        }

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height,
                        0, textureFormat, GL_UNSIGNED_BYTE, pix);
        SDL_FreeSurface(surface);

        addRectangle(&vertices, location, location +
                        btVector3(10, 10, 0));
        addRectangleUV(&uvs);

        glUniform1i(blProgramDebug->locHasTexture, 1);
        glEnableVertexAttribArray(blProgramDebug->locUV);
        glEnableVertexAttribArray(blProgramDebug->locVertices);

        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GL_FLOAT),
                        &uvs[0], GL_STATIC_DRAW);
        glVertexAttribPointer(blProgramDebug->locUV, 2, GL_FLOAT
                        , GL_FALSE, 0, (void *)0);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(btVector3),
                        &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(blProgramDebug->locVertices, 4 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glDisableVertexAttribArray(blProgramDebug->locUV);
        glDisableVertexAttribArray(blProgramDebug->locVertices);

        vertices.clear();
        uvs.clear();
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
        vertices.clear();
}

void BlDebugDrawer::finalizeDraw()
{
        glEnableVertexAttribArray(blProgramDebug->locColor);
        glEnableVertexAttribArray(blProgramDebug->locVertices);
        glUniform1i(blProgramDebug->locHasTexture, 0);

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
