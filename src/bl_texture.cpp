#include "bl_texture.h"
#include <bl_log.h>

GLuint BlTexture::fetchTexture(std::string filename)
{
        if(mapTextureToBuffer.count(filename) > 0) {
                GLuint texId = mapTextureToBuffer[filename];
                if (glIsTexture(texId)) {
                        printf("Texid %u for %s is a texture\n", texId, filename.c_str());
                        return texId;
                }
                mapTextureToBuffer.erase(filename);
        }
        INFO("Loading texture %s\n", filename.c_str());
        GLuint textureBuffer;
        glGenTextures(1, &textureBuffer);
        BlImage *blImage = readPngImage(filename.c_str());
        blImage->loadInBuffer(textureBuffer);
        delete blImage;
        mapTextureToBuffer[filename] = textureBuffer;
        return textureBuffer;
}

void BlTexture::deleteTexture(std::string filename)
{
        GLuint texId = mapTextureToBuffer[filename];
        glDeleteTextures(1, &texId);
        mapTextureToBuffer.erase(filename);
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
