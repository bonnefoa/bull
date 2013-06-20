#ifndef BL_IMAGE_H
#define BL_IMAGE_H

#define GRAY_CHANNEL 1
#define RGB_CHANNEL 3
#define RGBA_CHANNEL 4

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <vector>
#include <bullet.h>

class BlImage {
        public:
                BlImage (unsigned int _width,
                                unsigned int _height,
                                unsigned char * _pixels,
                                GLenum _format,
                                int _numChannels) :
                        width(_width),
                        height(_height),
                        pixels(_pixels),
                        format(_format),
                        numChannels(_numChannels) {};
                int getPixelIndexAt(int x, int y);
                btVector3 getPixelsAt(int x, int y);
                unsigned char getPixelAt(int x, int y);

                unsigned int width;
                unsigned int height;
                unsigned char *pixels;
                GLenum format;
                int numChannels;

                void loadInBuffer(GLuint imageBuffer);
                void writeImage(const char *destination);
                virtual ~BlImage(void);
};

BlImage *readPngImage(const char *filename);
BlImage *readMultipleImages(std::vector<std::string> images);

#endif
