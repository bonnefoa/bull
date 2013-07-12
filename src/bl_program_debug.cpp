#include "bl_program_debug.h"
#include <bl_matrix.h>
#include <bl_log.h>

BlProgramDebug *getProgramDebug()
{
        std::vector<BlShader*> shaders;
        BlShader *vertexShader = new BlShader("glsl/debug.vert"
                        , GL_VERTEX_SHADER);
        BlShader *fragmentShader = new BlShader("glsl/debug.frag"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(vertexShader);
        shaders.push_back(fragmentShader);

        BlProgramDebug *blProgramDebug = new BlProgramDebug(shaders);
        blProgramDebug->loadProgram();
        blProgramDebug->init();
        return blProgramDebug;
}

void BlProgramDebug::init()
{
        glUseProgram(programId);

        locView = glGetUniformLocation(programId, "V");
        locProjection = glGetUniformLocation(programId, "P");

        locVertices = glGetAttribLocation(programId, "vertexPosition_modelspace");
        locColor = glGetAttribLocation(programId, "vertexColor");
        samplerTexture = glGetUniformLocation(programId
                        , "textureSampler");
        INFO("Loc vertices %i, loc Color %i\n",
                        locVertices, locColor);
        if(locVertices < 0 || locColor < 0) {
                ERROR("A location is unused");
        }

        glUniform1i(samplerTexture, 5);
        glUseProgram(0);
}
