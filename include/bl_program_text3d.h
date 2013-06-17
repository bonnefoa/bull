#ifndef BL_PROGRAM_TEXT3D_H
#define BL_PROGRAM_TEXT3D_H

#include <bl_program.h>
#include <bl_config.h>

class BlProgramText3d : public BlProgram
{
        public:
                BlProgramText3d(std::vector<BlShader *> shaders, BlConfig *_blConfig) :
                       BlProgram(shaders), blConfig(_blConfig) {} ;

                void init(void);

                GLint locVertices;
                GLint locUV;
                GLint locSampler;

                GLint locView;
                GLint locProjection;

        private:
                BlConfig *blConfig;
};

BlProgramText3d *getProgramText3d(BlConfig *blConfig);

#endif
