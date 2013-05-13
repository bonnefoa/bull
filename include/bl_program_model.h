#ifndef BL_PROGRAM_MODEL_H
#define BL_PROGRAM_MODEL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bl_program.h>
#include <bl_shader.h>
#include <bl_model.h>
#include <bl_input.h>
#include <vector>

class  BlProgramModel : public BlProgram
{
        public:
                BlProgramModel(std::vector<BlShader *> shaders
                                , BlInput *blInput) :
                        BlProgram(shaders), blInput(blInput) {};
                void init(void);
                void bindModelMatrix(BlModel *blModel);
                void bindProjectionMatrix(void);
                void displayModel(BlModel *model);

                GLuint uniformLightPosition;
                GLuint uniformLightColor;
                GLuint uniformAmbientColor;

        private:
                BlInput *blInput;
                GLint uniformM;
                GLint uniformV;
                GLint uniformP;
                GLint locVertexPosModelspace;
                GLint locNormalModelspace;
                GLint locUV;
};

#endif
