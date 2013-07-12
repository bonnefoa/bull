#ifndef BL_PROGRAM_TEXT2D_H
#define BL_PROGRAM_TEXT2D_H

#include <bl_program.h>
#include <bl_config.h>

class BlProgramText2d : public BlProgram
{
        public:
                BlProgramText2d(std::vector<BlShader *> shaders, BlConfig *_blConfig) :
                       BlProgram(shaders), blConfig(_blConfig) {} ;

                void init(void);

                GLint locVertices;
                GLint locUV;
                GLint locSampler;

        private:
                BlConfig *blConfig;

                GLint locWidth;
                GLint locHeight;
};

BlProgramText2d *getProgramText2d(BlConfig *blConfig);

#endif
