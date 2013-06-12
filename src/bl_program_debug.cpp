#include "bl_program_debug.h"
#include <bl_matrix.h>
#include <bl_log.h>

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
        locHasTexture = glGetUniformLocation(programId, "hasTexture");

        locVertices = glGetAttribLocation(programId, "vertexPosition_modelspace");
        locColor = glGetAttribLocation(programId, "vertexColor");
	locUV = glGetAttribLocation(programId, "vertexUV");
        samplerTexture = glGetUniformLocation(programId
                        , "textureSampler");
        INFO("Loc vertices %i, loc Color %i, loc UV %i\n",
                        locVertices, locColor, locUV);
        if(locVertices < 0 || locColor < 0 || locUV < 0) {
                ERROR("A location is unused");
        }

        glUniform1i(samplerTexture, 5);

        bindProjectionMatrix(programId, locProjection, blConfig->projection);
}
