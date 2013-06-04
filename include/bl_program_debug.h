#ifndef BL_PROGRAM_DEBUG_H
#define BL_PROGRAM_DEBUG_H

#include <bl_program.h>
#include <bl_config.h>

class BlProgramDebug : public BlProgram
{
        public:
                BlProgramDebug(std::vector<BlShader *> shaders, BlConfig *_blConfig) :
                        BlProgram(shaders), blConfig(_blConfig) {};

                void init(void);

                GLint locView;
                GLint locProjection;
                GLint locHasTexture;

                GLint locVertices;
                GLint locColor;
                GLint locUV;
                GLint samplerTexture;
        private:
                BlConfig *blConfig;
};

BlProgramDebug *getProgramDebug(BlConfig *blConfig);

#endif
