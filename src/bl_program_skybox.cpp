#include "bl_program_skybox.h"
#include <bl_matrix.h>

BlProgramSkybox *getProgramSkybox(BlConfig *blConfig)
{
        std::vector<BlShader*> shaders;
        BlShader *vertexShader = new BlShader("glsl/skybox.vert"
                        , GL_VERTEX_SHADER);
        BlShader *fragmentShader = new BlShader("glsl/skybox.frag"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(vertexShader);
        shaders.push_back(fragmentShader);

        BlProgramSkybox *blProgramSkybox = new BlProgramSkybox(shaders,
                        blConfig);
        blProgramSkybox->loadProgram();
        blProgramSkybox->init();
        return blProgramSkybox;
}


void BlProgramSkybox::init()
{
        glUseProgram(programId);

        locVertices = glGetAttribLocation(programId
                        , "vertexPosition_modelspace");
        locModel = glGetUniformLocation(programId, "M");
        locView = glGetUniformLocation(programId, "V");
        locProjection = glGetUniformLocation(programId, "P");

        cubemapSampler = glGetUniformLocation(programId
                        , "cubemapSampler");
        glUniform1i(cubemapSampler, 10);
}

void BlProgramSkybox::displayScene(BlScene *scene, btTransform view)
{
        glUseProgram(programId);
        sendTransform(view, locView);
        scene->blSkybox->drawSkybox(locModel, locVertices);
}

void BlProgramSkybox::bindProjection()
{
        bindProjectionMatrix(programId, locProjection, blConfig->projection);
}