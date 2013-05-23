#include "bl_image.h"
#include <bl_log.h>
#include <png.h>

void BlImage::loadInBuffer(GLuint textureBuffer)
{
        glBindTexture(GL_TEXTURE_2D, textureBuffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        INFO("Loading in buffer %i image of size %i / %i\n", textureBuffer,
                        width, height);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                        width, height,
                        0, format, GL_UNSIGNED_BYTE,
                        pixels);
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

        int bit_depth;
        int color_type;
        png_uint_32  i, rowbytes;
        png_uint_32 width;
        png_uint_32 height;
        int numChannels = RGB_CHANNEL;
        GLenum format = GL_RGB;
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING
                        , NULL, NULL, NULL);
        png_infop info_ptr = png_create_info_struct(png_ptr);

        png_init_io(png_ptr, infile);
        png_set_sig_bytes(png_ptr, 8);
        png_read_info(png_ptr, info_ptr);
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
                        &color_type, NULL, NULL, NULL);
        if (color_type == PNG_COLOR_TYPE_PALETTE)
                png_set_expand(png_ptr);
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
                png_set_expand(png_ptr);
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
                png_set_expand(png_ptr);
        if (bit_depth == 16)
                png_set_strip_16(png_ptr);
        if (color_type == PNG_COLOR_TYPE_GRAY ||
                        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
                png_set_gray_to_rgb(png_ptr);
        if(color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
                numChannels = RGBA_CHANNEL;
                format = GL_RGBA;
        }

        png_bytep row_pointers[height];
        png_read_update_info(png_ptr, info_ptr);
        rowbytes = png_get_rowbytes(png_ptr, info_ptr);

        unsigned char *lines = (unsigned char *) malloc(sizeof(unsigned char)
                                * width * height * numChannels);
        for (i = 0;  i < height;  ++i)
                row_pointers[i] = lines + i*rowbytes;
        png_read_image(png_ptr, row_pointers);
        png_read_end(png_ptr, NULL);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

        return new BlImage(width, height, lines, format, numChannels);
}

BlImage::~BlImage()
{
        free(pixels);
}
