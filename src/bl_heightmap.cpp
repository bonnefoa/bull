#include "bl_heightmap.h"

BlImage *generateNormalMap(BlImage *blImage)
{
        int srcChannels = blImage->numChannels;
        unsigned width = blImage->width;
        unsigned height = blImage->height;
        unsigned char *pixels =
                (unsigned char *) malloc(width * height * RGB_CHANNEL);

        int srcLine = width * srcChannels;
        int curLine = width * RGB_CHANNEL;

        for(unsigned int x = 0; x < width; x++) {
                for(unsigned int y = 0; y < height; y++) {
                        int indexSrc = x * srcChannels
                                + y * srcLine;
                        int index = x * RGB_CHANNEL + y * curLine;

                        int heightYUp = blImage->pixels[indexSrc + srcLine];
                        int heightYDown = blImage->pixels[indexSrc - srcLine];
                        int heightXLeft = blImage->pixels[indexSrc - srcChannels];
                        int heightXRight = blImage->pixels[indexSrc + srcChannels];
                        btVector3 vecX = btVector3(1,
                                        0, heightXRight - heightXLeft);
                        btVector3 vecY = btVector3(0,
                                        1, heightYUp - heightYDown);
                        btVector3 res = vecX.cross(vecY);
                        res = res.normalize();
                        res = res + btVector3(1.0f,1.0f,1.0f);
                        res = res / btVector3(2.0f, 2.0f, 2.0f);

                        pixels[index] = res[0] * 255;
                        pixels[index+ 1] = res[1] * 255;
                        pixels[index+ 2] = res[2] * 255;
                }
        }
        return new BlImage(width, height, pixels, GL_RGB, RGB_CHANNEL);
}
