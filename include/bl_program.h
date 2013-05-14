#ifndef BL_PROGRAM_H
#define BL_PROGRAM_H

#include <bl_shader.h>
#include <vector>

class BlProgram
{
        public:
                GLuint programId;
                BlProgram(std::vector<BlShader *> shaders)
                        : shaders(shaders) {programId=0;};
                void loadProgram(void);

                ~BlProgram(void);
        private:
                std::vector<BlShader *> shaders;
};

#endif
