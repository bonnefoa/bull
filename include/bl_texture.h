#ifndef BL_TEXTURE_H
#define BL_TEXTURE_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <map>
#include <string>

class BlTexture
{
        public:
                BlTexture();
                ~BlTexture();
                GLuint fetchTexture(const char* filename);
                void clear();

        private:
                std::map<const char*, GLuint> mapTextureToBuffer;
};

#endif
