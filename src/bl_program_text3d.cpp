#include "bl_program_text3d.h"
#include <bl_log.h>
#include <bl_matrix.h>

BlProgramText3d *getProgramText3d(BlConfig *blConfig)
{
        std::vector<BlShader*> shaders;
        BlShader *vertexShader = new BlShader("glsl/text3d.vert"
                        , GL_VERTEX_SHADER);
        BlShader *fragmentShader = new BlShader("glsl/text3d.frag"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(vertexShader);
        shaders.push_back(fragmentShader);

        BlProgramText3d *blProgramText3d = new BlProgramText3d(shaders, blConfig);
        blProgramText3d->loadProgram();
        blProgramText3d->init();
        return blProgramText3d;
}

void BlProgramText3d::init()
{
        glUseProgram(programId);
        locVertices = glGetAttribLocation(programId,
                        "vertexPosition_modelspace");
        locUV = glGetAttribLocation(programId, "vertexUV");
        locSampler = glGetUniformLocation(programId, "textureSampler");

        locView = glGetUniformLocation(programId, "V");
        locProjection = glGetUniformLocation(programId, "P");

        if(locVertices < 0 || locUV < 0 || locSampler < 0) {
                ERROR("A location is unused");
        }

        glUniform1i(locSampler, 4);
        bindProjectionMatrix(programId, locProjection, blConfig->projection);

        glUseProgram(0);
}
