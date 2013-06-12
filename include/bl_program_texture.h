#ifndef BL_PROGRAM_TEXTURE_H
#define BL_PROGRAM_TEXTURE_H

#include <bl_program.h>
#include <GL/glew.h>
#include <GL/gl.h>

class BlProgramTexture : public BlProgram {
        public:
                BlProgramTexture(std::vector<BlShader *> shaders) :
                        BlProgram(shaders) {};
                void init(void);
                void displayTexture(GLuint textureId);
        private:
                GLuint quadVertexBuffer;
                GLint locVertices;
                GLint samplerTexture;
};

BlProgramTexture *getProgramTexture();

#endif
