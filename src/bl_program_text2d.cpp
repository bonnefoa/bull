#include "bl_program_text2d.h"
#include <bl_log.h>

BlProgramText2d *getProgramText2d(BlConfig *blConfig)
{
        std::vector<BlShader*> shaders;
        BlShader *vertexShader = new BlShader("glsl/text2d.vert"
                        , GL_VERTEX_SHADER);
        BlShader *fragmentShader = new BlShader("glsl/text2d.frag"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(vertexShader);
        shaders.push_back(fragmentShader);

        BlProgramText2d *blProgramText2d = new BlProgramText2d(shaders, blConfig);
        blProgramText2d->loadProgram();
        blProgramText2d->init();
        return blProgramText2d;
}

void BlProgramText2d::init()
{
        glUseProgram(programId);
        locVertices = glGetAttribLocation(programId,
                        "vertexPosition_screenspace");
        locUV = glGetAttribLocation(programId, "vertexUV");
        locWidth = glGetUniformLocation(programId, "width");
        locHeight = glGetUniformLocation(programId, "height");
        locSampler = glGetUniformLocation(programId, "textureSampler");

        if(locVertices < 0 || locUV < 0 || locWidth < 0 || locSampler < 0) {
                ERROR("A location is unused");
        }

        glUniform1i(locSampler, 4);

        glUniform1i(locWidth, blConfig->width / 2);
        glUniform1i(locHeight, blConfig->height / 2);

        glUseProgram(0);
}
