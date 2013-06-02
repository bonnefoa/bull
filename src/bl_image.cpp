#include "bl_image.h"
#include <bl_log.h>
#include <png.h>
#include <zlib.h>
#include <math.h>
#include <string.h>

int colorTypeToNumberChannels(int colorType)
{
        switch(colorType) {
                case PNG_COLOR_TYPE_GRAY:
                        return RGB_CHANNEL;
                case PNG_COLOR_TYPE_GRAY_ALPHA:
                        return RGBA_CHANNEL;
                case PNG_COLOR_TYPE_RGB:
                        return RGB_CHANNEL;
                case PNG_COLOR_TYPE_RGB_ALPHA:
                        return RGBA_CHANNEL;
        }
        ERROR("Unknown color type %i\n", colorType);
        return 0;
}

GLenum colorTypeToGlFormat(int colorType)
{
        switch(colorType) {
                case PNG_COLOR_TYPE_GRAY:
                        return GL_RGB;
                case PNG_COLOR_TYPE_GRAY_ALPHA:
                        return GL_RGBA;
                case PNG_COLOR_TYPE_RGB:
                        return GL_RGB;
                case PNG_COLOR_TYPE_RGB_ALPHA:
                        return GL_RGBA;
        }
        ERROR("Unknown color type %i\n", colorType);
        return 0;
}

BlImage *readMultipleImages(std::vector<std::string> images)
{
        std::vector<BlImage*> parsedImages;
        for (std::vector<std::string>::iterator it=images.begin();
                        it!=images.end(); ++it) {
                parsedImages.push_back(readPngImage((*it).c_str()));
        }

        unsigned int width = parsedImages[0]->width;
        unsigned int height = parsedImages[0]->height;
        GLenum format = parsedImages[0]->format;
        int numChannels = parsedImages[0]->numChannels;

        size_t imageSize = width * height * numChannels * sizeof(char);
        unsigned char *pixels = (unsigned char*) malloc(imageSize
                        * images.size());

        for (unsigned int i = 0; i < images.size(); i++) {
                memcpy(pixels + i * imageSize,
                       parsedImages[i]->pixels,
                       imageSize);
                delete parsedImages[i];
        }
        return new BlImage(width, height, pixels, format, numChannels);
}

BlImage *readPngImage(const char *filename)
{
        FILE * infile;
        if ((infile = fopen(filename, "rb")) == NULL) {
                ERROR("can't open '%s'\n", filename);
                return NULL;
        }
        unsigned char sig[8];
        fread(sig, 1, 8, infile);
        if (png_sig_cmp(sig, 0, 8) != 0) {
                ERROR("%s is not a png file\n", filename);
                return NULL;
        }

        int bitDepth;
        int colorType;
        png_uint_32  i, rowbytes;
        png_uint_32 width;
        png_uint_32 height;
        png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING
                        , NULL, NULL, NULL);
        png_infop infoPtr = png_create_info_struct(pngPtr);

        png_init_io(pngPtr, infile);
        png_set_sig_bytes(pngPtr, 8);
        png_read_info(pngPtr, infoPtr);
        png_get_IHDR(pngPtr, infoPtr, &width, &height, &bitDepth,
                        &colorType, NULL, NULL, NULL);
        if (colorType == PNG_COLOR_TYPE_PALETTE)
                png_set_expand(pngPtr);
        if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
                png_set_expand(pngPtr);
        if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
                png_set_expand(pngPtr);
        if (bitDepth == 16)
                png_set_strip_16(pngPtr);
        if (colorType == PNG_COLOR_TYPE_GRAY ||
                        colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
                png_set_gray_to_rgb(pngPtr);

        int numChannels = colorTypeToNumberChannels(colorType);
        GLenum format = colorTypeToGlFormat(colorType);

        png_bytep row_pointers[height];
        png_read_update_info(pngPtr, infoPtr);
        rowbytes = png_get_rowbytes(pngPtr, infoPtr);

        unsigned char *lines = (unsigned char *) malloc(sizeof(unsigned char)
                                * width * height * numChannels);
        for (i = 0;  i < height;  ++i)
                row_pointers[i] = lines + i*rowbytes;
        png_read_image(pngPtr, row_pointers);
        png_read_end(pngPtr, NULL);
        png_destroy_read_struct(&pngPtr, &infoPtr, NULL);

        unsigned int texWidth = pow(2, ceil(log2(width)));
        unsigned int texHeight = pow(2, ceil(log2(height)));
        if(texWidth == width && texHeight == height) {
                return new BlImage(width, height, lines, format, numChannels);
        }

        size_t sizePixels = sizeof(unsigned char) *
                texWidth * texHeight * numChannels;
        unsigned char *pixels = (unsigned char *) malloc(sizePixels);
        memset(pixels, 0, sizePixels);
        for (i = 0;  i < height; i++) {
                memcpy(pixels + i * texWidth * numChannels,
                       lines + i * width * numChannels,
                       width * numChannels * sizeof(char));
        }
        free(lines);
        return new BlImage(texWidth, texHeight, pixels, format, numChannels);
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
                        width, height);
        glTexImage2D(GL_TEXTURE_2D, 0, format,
                        width, height,
                        0, format, GL_UNSIGNED_BYTE,
                        pixels);
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
        png_set_IHDR(pngPtr, infoPtr, width,
                        height, 8,
                        PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        png_write_info(pngPtr, infoPtr);
        for(unsigned int i = 0; i < height; i++) {
                png_write_row(pngPtr,
                                &pixels[i * width * numChannels]);
        }
        png_write_end(pngPtr, NULL);
        fclose(outfile);
}

int BlImage::getPixelIndexAt(int x, int y)
{
        return x * numChannels + y * width * numChannels;
}

unsigned char BlImage::getPixelAt(int x, int y)
{
        return pixels[getPixelIndexAt(x, y)];
}


BlImage::~BlImage()
{
        free(pixels);
}
