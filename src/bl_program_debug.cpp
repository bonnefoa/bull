#include "bl_program_debug.h"
#include <bl_matrix.h>

BlProgramDebug *getProgramDebug(BlConfig *blConfig)
{
        std::vector<BlShader*> shaders;
        BlShader *vertexShader = new BlShader("glsl/debug_vertex.glsl"
                        , GL_VERTEX_SHADER);
        BlShader *fragmentShader = new BlShader("glsl/debug_fragment.glsl"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(vertexShader);
        shaders.push_back(fragmentShader);

        BlProgramDebug *blProgramDebug = new BlProgramDebug(shaders, blConfig);
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

        bindProjectionMatrix(programId, locProjection, blConfig->projection);
}
