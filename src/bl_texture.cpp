#include "bl_texture.h"
#include <bl_log.h>

BlTexture::BlTexture()
{
}

GLuint BlTexture::fetchTexture(std::string filename)
{
        if(mapTextureToBuffer.count(filename) > 0) {
                return mapTextureToBuffer[filename];
        }

        GLuint textureBuffer;
        glGenTextures(1, &textureBuffer);
        BlImage *blImage = readPngImage(filename.c_str());
        blImage->loadInBuffer(textureBuffer);
        delete blImage;
        mapTextureToBuffer[filename] = textureBuffer;
        return textureBuffer;
}

GLuint BlTexture::fillTextureAtlas(std::string setName
                , std::vector<std::string> files)
{
        BlImage *blImage = readMultipleImages(files);

        GLuint textureBuffer;
        glGenTextures(1, &textureBuffer);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        INFO("Loading in buffer %i image of size %i / %i\n",
                        textureBuffer, blImage->width, blImage->height);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                        blImage->width, blImage->height,
                        0, GL_RGB, GL_UNSIGNED_BYTE,
                        blImage->pixels);

        mapTextureToBuffer[setName] = textureBuffer;
        return textureBuffer;

}

void BlTexture::clear()
{
        std::map<std::string, GLuint>::iterator it;
        for (it=mapTextureToBuffer.begin(); it!=mapTextureToBuffer.end(); ++it) {
                glDeleteBuffers(1, &it->second);
        }
        mapTextureToBuffer.clear();
}

BlTexture::~BlTexture()
{
        clear();
}
