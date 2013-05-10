#ifndef BL_IMAGE_H
#define BL_IMAGE_H

#define RGB_CHANNEL 3
#define RGBA_CHANNEL 4

#include <GL/glew.h>
#include <GL/gl.h>


class BlImage {
        public:
                BlImage (unsigned int _width,
                                unsigned int _height,
                                unsigned char * _pixels,
                                GLenum _format) :
                        width(_width),
                        height(_height),
                        pixels(_pixels),
                        format(_format) {};

                unsigned int width;
                unsigned int height;
                unsigned char *pixels;
                GLenum format;
};

#endif
