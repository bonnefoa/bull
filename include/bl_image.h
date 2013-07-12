#ifndef BL_IMAGE_H
#define BL_IMAGE_H

#define GRAY_CHANNEL 1
#define RGB_CHANNEL 3
#define RGBA_CHANNEL 4

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <SDL_image.h>
#include "bullet.h"

class BlImage {
        public:
                BlImage (SDL_Surface *_surface) : surface(_surface) {};
                BlImage (const char *filename);

                int getPixelIndexAt(int x, int y);
                btVector3 getPixelsAt(int x, int y);
                unsigned char getPixelAt(int x, int y);

                void loadInBuffer(GLuint imageBuffer);
                void writeImage(const char *destination);
                virtual ~BlImage(void);

                SDL_Surface *surface;

                void loadInCubeMap(GLenum target);
        private:
                GLenum getGlFormat();
                GLenum getSurfaceFormat();
                bool isTga(const char *filename);
};

#endif
