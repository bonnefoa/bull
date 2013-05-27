#ifndef BL_TEXTURE_H
#define BL_TEXTURE_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <map>
#include <vector>
#include <string>
#include <bl_image.h>

class BlTexture
{
        public:
                BlTexture();
                ~BlTexture();
                GLuint fetchTexture(std::string filename);
                GLuint fill3dTexture(std::string setName
                        , std::vector<std::string> files);
                void clear();

        private:
                std::map<std::string, GLuint> mapTextureToBuffer;
};

#endif
