#include "bl_texture.h"
#include <bl_image.h>

BlTexture::BlTexture()
{
}

GLuint BlTexture::fetchTexture(const char* filename)
{
        std::map<const char*, GLuint>::iterator it;
        it = mapTextureToBuffer.find(filename);
        if(it != mapTextureToBuffer.end()) {
                return it->second;
        }

        GLuint textureBuffer;
        glGenTextures(1, &textureBuffer);
        BlImage *blImage = readPngImage(filename);
        blImage->loadInBuffer(textureBuffer);
        delete blImage;
        mapTextureToBuffer[filename] = textureBuffer;
        return textureBuffer;
}

void BlTexture::clear()
{
        std::map<const char*, GLuint>::iterator it;
        for (it=mapTextureToBuffer.begin(); it!=mapTextureToBuffer.end(); ++it) {
                glDeleteBuffers(1, &it->second);
        }
        mapTextureToBuffer.clear();
}

BlTexture::~BlTexture()
{
        clear();
}
