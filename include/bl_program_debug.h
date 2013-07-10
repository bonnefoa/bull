#ifndef BL_PROGRAM_DEBUG_H
#define BL_PROGRAM_DEBUG_H

#include <bl_program.h>
#include <bl_config.h>

class BlProgramDebug : public BlProgram
{
        public:
                BlProgramDebug(std::vector<BlShader *> shaders) :
                        BlProgram(shaders) {};

                void init(void);

                GLint locView;
                GLint locProjection;

                GLint locVertices;
                GLint locColor;
                GLint samplerTexture;
        private:
};

BlProgramDebug *getProgramDebug();

#endif
