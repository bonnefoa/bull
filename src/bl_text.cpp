#include "bl_text.h"
#include <bl_util.h>
#include <bl_image.h>
#include <bl_vertice.h>
#include <bl_log.h>

void BlText::init()
{
        (void) blConfig;

        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &uvBuffer);
        glGenTextures(1, &textureBuffer);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        addRectangleUV(&textUvs);
}

BlText::~BlText()
{
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &uvBuffer);
        glDeleteTextures(1, &textureBuffer);
}

void BlText::fillTextTexture(const char *text, float *width, float *height)
{
        SDL_Color textColor = {255,255,255,1};
        SDL_Color bg = {0,0,0,0};
        SDL_Surface *surface =
                TTF_RenderText_Shaded(blState->font, text, textColor, bg);
        GLenum textureFormat = GL_RGBA;
        *width =  float(surface->w);
        *height = float(surface->h);

        int textureWidth = roundUpPowerOfTwo(surface->w);
        int textureHeight = roundUpPowerOfTwo(surface->h);
        unsigned char *surfPix = (unsigned char *)surface->pixels;
        unsigned char pix[textureWidth * textureHeight * sizeof(unsigned char) * RGBA_CHANNEL];
        memset(pix, 0, textureWidth * textureHeight * sizeof(unsigned char) * RGBA_CHANNEL);
        for(int x=0; x < surface->w; x++) {
                for(int y=0; y < surface->h; y++) {
                        int indexSrc = x + (surface-> h - 1 - y) * surface->pitch;
                        int index = x * RGBA_CHANNEL + y * textureWidth * RGBA_CHANNEL;
                        int val = surfPix[indexSrc];
                        pix[index] = val;
                        pix[index+1] = val;
                        pix[index+2] = val;
                        pix[index+3] = val;
                }
        }

        glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, textureWidth, textureHeight,
                        0, textureFormat, GL_UNSIGNED_BYTE, pix);
        SDL_FreeSurface(surface);
}

void BlText::bindUV(GLint locUV)
{
        glEnableVertexAttribArray(locUV);

        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, textUvs.size() * sizeof(GL_FLOAT),
                        &textUvs[0], GL_STATIC_DRAW);
        glVertexAttribPointer(blProgramText2d->locUV, 2, GL_FLOAT
                        , GL_FALSE, 0, (void *)0);
}

void BlText::print3dText(const btVector3& location,const char* textString)
{
        float width;
        float height;
        std::vector<btVector3> textVertices;

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);

        fillTextTexture(textString, &width, &height);

        addRectangle(&textVertices, location, location +
                        btVector3(20, 5.0f * width / height, 0));
        addRectangleUV(&textUvs);

        bindUV(blProgramText3d->locUV);

        glEnableVertexAttribArray(blProgramText3d->locVertices);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, textVertices.size() * sizeof(btVector3),
                        &textVertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(blProgramText3d->locVertices, 4 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);

        glDrawArrays(GL_TRIANGLES, 0, textVertices.size());

        glDisableVertexAttribArray(blProgramText3d->locUV);
        glDisableVertexAttribArray(blProgramText3d->locVertices);

        textVertices.clear();
        textUvs.clear();

}

void BlText::print2dText(const char *text, const float screenX, const float screenY)
{
        float height;
        float width;
        std::vector<float> textVertices;

        glUseProgram(blProgramText2d->programId);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);
        fillTextTexture(text, &width, &height);

        add2dRectangle(&textVertices, screenX, screenY, width, height);

        bindUV(blProgramText2d->locUV);

        glEnableVertexAttribArray(blProgramText2d->locVertices);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, textVertices.size() * sizeof(GL_FLOAT),
                        &textVertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(blProgramText2d->locVertices, 2 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(blProgramText2d->locVertices);
        glDisableVertexAttribArray(blProgramText2d->locUV);

        glUseProgram(0);
}
