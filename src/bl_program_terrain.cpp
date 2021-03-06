#include "bl_program_terrain.h"
#include <bl_matrix.h>
#include <bl_log.h>

BlProgramTerrain *getProgramTerrain(BlConfig *blConfig)
{
        std::vector<BlShader*> shaders;
        BlShader *vertexShader = new BlShader("glsl/terrain.vert"
                        , GL_VERTEX_SHADER);
        BlShader *fragmentShader = new BlShader("glsl/terrain.frag"
                        , GL_FRAGMENT_SHADER);
        BlShader *commonVertex = new BlShader("glsl/common_light.vert"
                        , GL_VERTEX_SHADER);
        BlShader *commonFragment = new BlShader("glsl/common_light.frag"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(vertexShader);
        shaders.push_back(fragmentShader);
        shaders.push_back(commonVertex);
        shaders.push_back(commonFragment);

        BlProgramTerrain *blProgramTerrain = new BlProgramTerrain(shaders,
                        blConfig);
        blProgramTerrain->loadProgram();
        blProgramTerrain->init();
        return blProgramTerrain;
}

void BlProgramTerrain::init(void)
{
        glUseProgram(programId);

        locModel = glGetUniformLocation(programId, "M");
        locView = glGetUniformLocation(programId, "V");
        locProjection = glGetUniformLocation(programId, "P");
        locUVTexture = glGetAttribLocation(programId, "uvTexture");
        locVertices = glGetAttribLocation(programId
                        , "vertexPosition_modelspace");
        locNormal = glGetAttribLocation(programId
                        , "vertexNormal_modelspace");
        locTangent = glGetAttribLocation(programId
                        , "vertexTangent_modelspace");
        locBitangent = glGetAttribLocation(programId
                        , "vertexBitangent_modelspace");
        textureSampler = glGetUniformLocation(programId
                        , "textureSampler");
        normalmapSampler = glGetUniformLocation(programId
                        , "normalmapSampler");
        heightmapSampler = glGetUniformLocation(programId
                        , "heightmapSampler");

        glUniform1i(heightmapSampler, 0);
        glUniform1i(textureSampler, 1);
        glUniform1i(normalmapSampler, 2);

        INFO("Model location %i\n", locModel);
        INFO("View location %i\n", locView);
        INFO("Projection location %i\n", locProjection);
        INFO("Vertices location %i\n", locVertices);
        INFO("Tangent location %i\n", locTangent);
        INFO("Bitangent location %i\n", locBitangent);
        INFO("UV texture location %i\n", locUVTexture);
        if(locModel < 0 || locView < 0 || locProjection < 0 || locVertices < 0
                        || locTangent < 0 || locBitangent < 0 ) {
                ERROR("unused location\n");
        }
        glUseProgram(0);
}

void BlProgramTerrain::displayScene(BlScene *blScene, btTransform view,
                btTransform projection)
{
        glUseProgram(programId);
        bindProjectionMatrix(locProjection, projection);
        sendTransform(view, locView);
        for (std::vector<BlTerrain*>::iterator
                        it = blScene->blTerrains->begin();
                        it != blScene->blTerrains->end(); ++it) {
                (*it)->drawElement(locModel,
                                locVertices,
                                locNormal,
                                locTangent,
                                locBitangent,
                                locUVTexture);
        }
        glUseProgram(0);
}
