#include "bl_image.h"

#include <bl_log.h>
#include <string.h>
#include <bl_util.h>
#include <png.h>
#include <zlib.h>
#include <math.h>

bool BlImage::isTga(const char *filename)
{
        const char *extension = filename + strlen(filename) - 4;
        int res = strncmp(extension, ".tga", 4);
        return res == 0;
}

BlImage::BlImage (const char *filename)
{
        SDL_RWops *rwop;
        rwop = SDL_RWFromFile(filename, "rb");
        bool isReverted = IMG_isPNG(rwop);

        if(isTga(filename)){
                surface = IMG_LoadTyped_RW(rwop, 1, "TGA");
        } else {
                surface = IMG_Load_RW(rwop, 1);
        }
        if (!surface) {
                INFO("Error on loading image %s\n", filename);
                INFO("IMG_Load: %s\n", IMG_GetError());
        }
        if (isReverted) {
                size_t lineSize = surface->format->BytesPerPixel * surface->w;
                unsigned char upPixels[lineSize * surface->h / 2];
                unsigned char *pixels = (unsigned char *)surface->pixels;
                for (int i = 0; i < surface->h / 2; i++) {
                        int downIndex = surface->h - i - 1;
                        memcpy(upPixels, pixels + i * lineSize, lineSize);
                        memcpy(pixels + i * lineSize,
                                        pixels + downIndex * lineSize,
                                        lineSize);
                        memcpy(pixels + downIndex * lineSize,
                                        upPixels,
                                        lineSize);
                }
        }
};

GLenum BlImage::getGlFormat()
{
        switch(surface->format->BytesPerPixel) {
                case 1:
                        return GL_LUMINANCE;
                case 3:
                        return GL_RGB;
                case 4:
                        return GL_RGBA;
        }
        ERROR("Unknown color type %i\n", surface->format->BytesPerPixel);
        return 0;
}

GLenum BlImage::getSurfaceFormat()
{
        Uint32 r = surface->format->Rmask;
        Uint32 g = surface->format->Gmask;
        Uint32 b = surface->format->Bmask;
        bool hasAlpha = surface->format->BytesPerPixel == 4;
        if(r == 0xff && g == 0xff00 && b == 0xff0000) {
                if(hasAlpha) {
                        return GL_RGBA;
                }
                return GL_RGB;
        }
        if(r == 0xff0000 && g == 0xff00 && b == 0xff) {
                if(hasAlpha) {
                        return GL_BGRA;
                }
                return GL_BGR;
        }
        INFO("Unkown pixel color order\n");
        return GL_RGB;
}

void BlImage::loadInBuffer(GLuint textureBuffer)
{
        glBindTexture(GL_TEXTURE_2D, textureBuffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        INFO("Loading in buffer %i image of size %i / %i\n", textureBuffer,
                        surface->w, surface->h);
        GLenum format = getGlFormat();
        GLenum surfaceFormat = getSurfaceFormat();
        glTexImage2D(GL_TEXTURE_2D, 0, format,
                        surface->w, surface->h,
                        0, surfaceFormat, GL_UNSIGNED_BYTE,
                        surface->pixels);
}

void BlImage::loadInCubeMap(GLenum target)
{
        GLenum format = getGlFormat();
        GLenum surfaceFormat = getSurfaceFormat();
        glTexImage2D(target, 0, format,
                        surface->w, surface->h, 0, surfaceFormat,
                        GL_UNSIGNED_BYTE, surface->pixels);
}

void BlImage::writeImage(const char *destination)
{
        FILE *outfile;
        if ((outfile = fopen(destination, "wb")) == NULL) {
                ERROR("can't open %s", destination);
        }
        png_structp  pngPtr;
        png_infop  infoPtr;
        pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                        NULL, NULL, NULL);
        infoPtr = png_create_info_struct(pngPtr);
        png_init_io(pngPtr, outfile);
        png_set_compression_level(pngPtr, Z_BEST_COMPRESSION);
        png_set_IHDR(pngPtr, infoPtr, surface->w,
                        surface->h, 8,
                        PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        png_write_info(pngPtr, infoPtr);
        unsigned char *pixels = (unsigned char *)surface->pixels;
        for(int i = 0; i < surface->h; i++) {
                png_write_row(pngPtr,
                                &pixels[(surface->h - 1 - i)
                                * surface->w * surface->format->BytesPerPixel]);
        }
        png_write_end(pngPtr, NULL);
        fclose(outfile);
}

int BlImage::getPixelIndexAt(int x, int y)
{
        int numChannels = surface->format->BytesPerPixel;
        return x * numChannels + y * surface->w * numChannels;
}

unsigned char BlImage::getPixelAt(int x, int y)
{
        return ((unsigned char *) surface->pixels)[getPixelIndexAt(x, y)];
}

btVector3 BlImage::getPixelsAt(int x, int y)
{
        unsigned char *pixels = (unsigned char *)surface->pixels;
        int index = getPixelIndexAt(x, y);
        float r = pixels[index] / 255.0f;
        float g = pixels[index + 1] / 255.0f;
        float b = pixels[index + 2] / 255.0f;
        return btVector3(r, g, b);
}

BlImage::~BlImage()
{
        SDL_FreeSurface(surface);
}
