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
                BlTexture() {};
                ~BlTexture();
                GLuint fetchTexture(std::string filename);
                void deleteTexture(std::string filename) ;
                void clear();

        private:
                std::map<std::string, GLuint> mapTextureToBuffer;
};

#endif
