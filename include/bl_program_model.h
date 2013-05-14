#ifndef BL_PROGRAM_MODEL_H
#define BL_PROGRAM_MODEL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bl_program.h>
#include <bl_shader.h>
#include <bl_input.h>
#include <vector>
#include <bl_scene.h>

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
                void displayScene(BlScene *blScene);

        private:
                BlInput *blInput;
                GLint uniformM;
                GLint uniformV;
                GLint uniformP;
                GLint locVertices;
                GLint locNormals;
                GLint locUVs;
};

BlProgramModel *getProgramModel(BlInput *blInput);

#endif
