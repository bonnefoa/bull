#include "bl_program_text2d.h"

BlProgramText2d *getProgramText2d(BlConfig *blConfig)
{
        std::vector<BlShader*> shaders;
        BlShader *vertexShader = new BlShader("glsl/text2d_vertex.glsl"
                        , GL_VERTEX_SHADER);
        BlShader *fragmentShader = new BlShader("glsl/text2d_fragment.glsl"
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

        glUniform1i(locWidth, blConfig->width);
        glUniform1i(locHeight, blConfig->height);

        glUseProgram(0);
}
