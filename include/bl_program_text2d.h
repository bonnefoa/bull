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

                GLuint locVertices;
                GLuint locUV;

        private:
                BlConfig *blConfig;

                GLuint locWidth;
                GLuint locHeight;
};

BlProgramText2d *getProgramText2d(BlConfig *blConfig);

#endif
