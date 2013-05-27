#include "bl_program_terrain.h"
#include <bl_matrix.h>
#include <bl_log.h>

BlProgramTerrain *getProgramTerrain(BlConfig *blConfig, BlState *blState)
{
        std::vector<BlShader*> shaders;
        BlShader *vertexShader = new BlShader("glsl/terrain_vertex.glsl"
                        , GL_VERTEX_SHADER);
        BlShader *fragmentShader = new BlShader("glsl/terrain_fragment.glsl"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(vertexShader);
        shaders.push_back(fragmentShader);

        BlProgramTerrain *blProgramTerrain = new BlProgramTerrain(shaders,
                        blConfig, blState);
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
        locGridLenght = glGetUniformLocation(programId, "gridLenght");
        locGridWidth = glGetUniformLocation(programId, "gridWidth");
        locUVTexture = glGetAttribLocation(programId, "locUVTexture");
        locVertices = glGetAttribLocation(programId
                        , "vertexPosition_modelspace");
        textureSampler = glGetUniformLocation(programId
                        , "textureSampler");
        heightmapSampler = glGetUniformLocation(programId
                        , "heightmapSampler");
        glUniform1i(heightmapSampler, 0);
        glUniform1i(textureSampler, 1);

        INFO("Model location %i\n", locModel);
        INFO("View location %i\n", locView);
        INFO("Projection location %i\n", locProjection);
        INFO("Grid width location %i\n", locGridWidth);
        INFO("Grid lenght location %i\n", locGridLenght);
        INFO("Vertices location %i\n", locVertices);
        INFO("UV texture location %i\n", locUVTexture);
        if(locModel < 0 || locView < 0 || locProjection < 0 ||
                        locGridWidth < 0 ||
                        locGridLenght < 0 ||
                        locVertices < 0){
                ERROR("unused location\n");
        }
}

void BlProgramTerrain::displayTerrain(BlTerrain *blTerrain)
{
        blTerrain->bindModelMatrix(locModel);
        blTerrain->bindVertices(locVertices);
        blTerrain->bindTextures();
        blTerrain->bindGridSize(locGridLenght, locGridWidth);
        blTerrain->drawElement();
}

void BlProgramTerrain::bindProjection()
{
        bindProjectionMatrix(programId, locProjection, blConfig->projection);
}

void BlProgramTerrain::displayScene(BlScene *blScene)
{
        glUseProgram(programId);
        sendTransform(blState->view, locView);
        for (std::vector<BlTerrain*>::iterator
                        it = blScene->blTerrains->begin();
                        it != blScene->blTerrains->end(); ++it) {
                displayTerrain(*it);
        }
}
