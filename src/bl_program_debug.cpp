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

void BlProgramDebug::bindProjection()
{
        bindProjectionMatrix(programId, locProjection, blConfig->projection);
}

void BlProgramDebug::init()
{
        glUseProgram(programId);

        locModel = glGetUniformLocation(programId, "locModel");
        locView = glGetUniformLocation(programId, "locView");
        locProjection = glGetUniformLocation(programId, "locProjection");

        locVertices = glGetAttribLocation(programId, "locVertices");
        locColor = glGetAttribLocation(programId, "locColor");
}
