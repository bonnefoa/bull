#include "bl_program_model.h"
#include <bl_util.h>
#include <bl_light_point.h>
#include <bullet.h>
#include <bl_log.h>
#include <bl_matrix.h>

BlProgramModel *getProgramModel(BlInput *blInput, BlConfig *blConfig, BlState *blState)
{
        std::vector<BlShader*> shaders;
        BlShader *vertexShader = new BlShader("glsl/model_vertex.glsl"
                        , GL_VERTEX_SHADER);
        BlShader *fragmentShader = new BlShader("glsl/model_fragment.glsl"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(vertexShader);
        shaders.push_back(fragmentShader);

        BlProgramModel *blProgramModel = new BlProgramModel(shaders, blInput, blConfig,
                        blState);
        blProgramModel->loadProgram();
        blProgramModel->init();
        return blProgramModel;
}

void BlProgramModel::init()
{
        glUseProgram(programId);

        locModel = glGetUniformLocation(programId, "M");
        locView = glGetUniformLocation(programId, "V");
        locProjection = glGetUniformLocation(programId, "P");
        locShadowVP = glGetUniformLocation(programId, "shadowVP");
        locVertices = glGetAttribLocation(programId
                        , "vertexPosition_modelspace");
        locNormals = glGetAttribLocation(programId
                        , "vertexNormal_modelspace");
        locUVs = glGetAttribLocation(programId
                        , "vertexUV");
        samplerTexture = glGetUniformLocation(programId
                        , "textureSampler");
        samplerShadow = glGetUniformLocation(programId
                        , "shadowSampler");
        glUniform1i(samplerTexture, 0);
        glUniform1i(samplerShadow, 2);
        INFO("vertex location %i\n", locVertices);
        INFO("normal location %i\n", locNormals);
        INFO("uv location %i\n", locUVs);
        INFO("shadow vp %i\n", locShadowVP);
        INFO("texture sampler %i\n", samplerTexture);
        INFO("shadow sampler %i\n", samplerShadow);
        if(locNormals < 0 || locUVs < 0 || locVertices < 0 || locShadowVP < 0
                        || samplerTexture < 0 || samplerShadow < 0 ){
                ERROR("A location is unused");
        }
}

void BlProgramModel::bindProjection()
{
        bindProjectionMatrix(programId, locProjection, blConfig->projection);
}

void BlProgramModel::displayModel(BlModel *blModel)
{
        blModel->bindModelMatrix(locModel);

        blModel->bindVertices(locVertices);
        blModel->bindNormals(locNormals);
        blModel->bindUVs(locUVs);
        blModel->drawElement();
}

void BlProgramModel::moveLight(btVector3 position)
{
        glUseProgram(programId);
        btTransform trans = computeVPShadowMatrix(position);
        sendTransform(trans, locShadowVP);
}

void BlProgramModel::displayScene(BlScene *blScene, GLuint depthTexture)
{
        glUseProgram(programId);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthTexture);

        sendTransform(blState->view, locView);

        for (std::vector<BlLightPoint*>::iterator
                        it = blScene->blLightPoints->begin();
                        it != blScene->blLightPoints->end(); ++it) {
                std::vector<BlModel*> *models = (*it)->blModels;
                for (std::vector<BlModel*>::iterator it2 = models->begin();
                                it2 != models->end(); ++it2) {
                        displayModel(*it2);
                }
        }
        for (std::vector<BlModel*>::iterator it = blScene->blModels->begin();
                        it != blScene->blModels->end(); ++it) {
                displayModel((*it));
        }
        for (std::vector<BlModel*>::iterator
                        it = blScene->blCharacter->blModels->begin();
                        it != blScene->blCharacter->blModels->end(); ++it) {
                displayModel((*it));
        }


        glDisableVertexAttribArray(locVertices);
        glDisableVertexAttribArray(locNormals);
        glDisableVertexAttribArray(locUVs);
}
