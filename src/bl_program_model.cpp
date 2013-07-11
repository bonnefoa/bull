#include "bl_program_model.h"
#include <bl_util.h>
#include <bl_light_point.h>
#include <bullet.h>
#include <bl_log.h>
#include <bl_matrix.h>

BlProgramModel *getProgramModel(BlConfig *blConfig,
                BlState *blState)
{
        std::vector<BlShader*> shaders;
        BlShader *vertexShader = new BlShader("glsl/model.vert"
                        , GL_VERTEX_SHADER);
        BlShader *commonVertex = new BlShader("glsl/common_light.vert"
                        , GL_VERTEX_SHADER);
        BlShader *commonFragment = new BlShader("glsl/common_light.frag"
                        , GL_FRAGMENT_SHADER);
        BlShader *fragmentShader = new BlShader("glsl/model.frag"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(vertexShader);
        shaders.push_back(fragmentShader);
        shaders.push_back(commonVertex);
        shaders.push_back(commonFragment);

        BlProgramModel *blProgramModel = new BlProgramModel(shaders,
                        blConfig,
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
        locTangent = glGetAttribLocation(programId
                        , "vertexTangent_modelspace");
        locBitangent = glGetAttribLocation(programId
                        , "vertexBitangent_modelspace");
        diffuseSampler = glGetUniformLocation(programId
                        , "diffuseSampler");
        normalSampler = glGetUniformLocation(programId
                        , "normalSampler");
        samplerShadow = glGetUniformLocation(programId
                        , "shadowSampler");
        glUniform1i(diffuseSampler, 0);
        glUniform1i(normalSampler, 1);
        glUniform1i(samplerShadow, 2);
        INFO("model location %i\n", locModel);
        INFO("vertex location %i\n", locVertices);
        INFO("normal location %i\n", locNormals);
        INFO("uv location %i\n", locUVs);
        INFO("shadow vp %i\n", locShadowVP);
        INFO("texture sampler %i\n", diffuseSampler);
        INFO("shadow sampler %i\n", samplerShadow);
        INFO("normal sampler %i\n", normalSampler);
        if(locModel < 0 || locNormals < 0
                        || locUVs < 0 || locVertices < 0
                        || locShadowVP < 0
                        || diffuseSampler < 0 || samplerShadow < 0 ){
                ERROR("A location is unused");
        }
        glUseProgram(0);
}

void BlProgramModel::moveLight(btVector3 position)
{
        glUseProgram(programId);
        btTransform trans = computeVPShadowMatrix(position);
        sendTransform(trans, locShadowVP);
        glUseProgram(0);
}

void BlProgramModel::displayScene(BlScene *blScene, GLuint depthTexture, btTransform view,
                btTransform projection)
{
        glUseProgram(programId);

        bindProjectionMatrix(programId, locProjection, projection);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthTexture);

        sendTransform(view, locView);

        for (std::vector<BlLightPoint*>::iterator
                        it = blScene->blLightPoints->begin();
                        it != blScene->blLightPoints->end(); ++it) {
                std::vector<BlModel*> *models = (*it)->blModels;
                for (std::vector<BlModel*>::iterator it2 = models->begin();
                                it2 != models->end(); ++it2) {
                        (*it2)->drawElement(locModel, locVertices,
                                        locNormals, locUVs,
                                        locTangent, locBitangent);
                }
        }
        for (std::vector<BlModel*>::iterator it = blScene->blModels->begin();
                        it != blScene->blModels->end(); ++it) {
                (*it)->drawElement(locModel, locVertices,
                                locNormals, locUVs,
                                locTangent, locBitangent);
        }
        blScene->blCharacter->drawCharacter(locModel, locVertices, locNormals, locUVs,
                        locTangent, locBitangent);

        glDisableVertexAttribArray(locVertices);
        glDisableVertexAttribArray(locNormals);
        glDisableVertexAttribArray(locUVs);
        glUseProgram(0);
}
