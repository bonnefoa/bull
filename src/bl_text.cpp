#include "bl_text.h"
#include <bl_util.h>
#include <bl_image.h>
#include <bl_vertice.h>

void BlText::init()
{
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &uvBuffer);
        glGenTextures(1, &textureBuffer);
}

BlText::~BlText()
{
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &uvBuffer);
        glDeleteTextures(1, &textureBuffer);
}

void BlText::print2dText(const char *text, const int x, const int y)
{

        SDL_Color textColor = {255,255,255,1};
        SDL_Color bg = {0,0,0,0};
        SDL_Surface *surface =
                TTF_RenderText_Shaded(blState->font, text, textColor, bg);
        GLenum textureFormat = GL_RGBA;
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
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, textureWidth, textureHeight,
                        0, textureFormat, GL_UNSIGNED_BYTE, pix);

        std::vector<float> textVertices;
        std::vector<float> textUvs;

        float height = float(surface->h);
        float width = float(surface->w);

        textVertices.push_back(x);
        textVertices.push_back(y);

        textVertices.push_back(x);
        textVertices.push_back(y + height);

        textVertices.push_back(x + width);
        textVertices.push_back(y + height);

        addRectangleUV(&textUvs);
        SDL_FreeSurface(surface);

        glEnableVertexAttribArray(blProgramText2d->locUV);
        glEnableVertexAttribArray(blProgramText2d->locVertices);

        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, textUvs.size() * sizeof(GL_FLOAT),
                        &textUvs[0], GL_STATIC_DRAW);
        glVertexAttribPointer(blProgramText2d->locUV, 2, GL_FLOAT
                        , GL_FALSE, 0, (void *)0);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, textVertices.size() * sizeof(btVector3),
                        &textVertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(blProgramText2d->locVertices, 4 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);

        glDrawArrays(GL_TRIANGLES, 0, textVertices.size());

        glDisableVertexAttribArray(blProgramText2d->locUV);
        glDisableVertexAttribArray(blProgramText2d->locVertices);
}
