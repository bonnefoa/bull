#include "bl_program_model.h"
#include <bl_util.h>
#include <bl_light_point.h>
#include <bullet.h>
#include <bl_log.h>
#include <bl_matrix.h>

BlProgramModel *getProgramModel(BlInput *blInput)
{
        std::vector<BlShader*> shaders;
        BlShader *vertexShader = new BlShader("glsl/model_vertex.glsl"
                        , GL_VERTEX_SHADER);
        BlShader *fragmentShader = new BlShader("glsl/model_fragment.glsl"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(vertexShader);
        shaders.push_back(fragmentShader);

        BlProgramModel *blProgramModel = new BlProgramModel(shaders, blInput);
        blProgramModel->loadProgram();
        blProgramModel->init();
        return blProgramModel;
}

void BlProgramModel::init()
{
        glUseProgram(programId);

        uniformM = glGetUniformLocation(programId, "M");
        uniformV = glGetUniformLocation(programId, "V");
        uniformP = glGetUniformLocation(programId, "P");
        locVertices = glGetAttribLocation(programId
                        , "vertexPosition_modelspace");
        locNormals = glGetAttribLocation(programId
                        , "vertexNormal_modelspace");
        locUVs = glGetAttribLocation(programId
                        , "vertexUV");
        INFO("vertex location %i\n", locVertices);
        INFO("normal location %i\n", locNormals);
        INFO("uv location %i\n", locUVs);
        if(locNormals < 0 || locUVs < 0 || locVertices < 0){
                ERROR("A location is unused");
        }
}

void BlProgramModel::bindProjectionMatrix()
{
        glUseProgram(programId);

        btScalar mat[16];
        blInput->projection.getOpenGLMatrix(mat);
        mat[11] = -1.0f;
        glUniformMatrix4fv(uniformP, 1, GL_FALSE, mat);
}

void BlProgramModel::displayModel(BlModel *blModel)
{
        blModel->bindModelMatrix(uniformM);
        sendTransform(blInput->view, uniformV);

        blModel->bindVertices(locVertices);
        blModel->bindNormals(locNormals);
        blModel->bindUVs(locUVs);
        blModel->drawElement();
}

void BlProgramModel::displayScene(BlScene *blScene)
{
        glUseProgram(programId);

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

        glDisableVertexAttribArray(locVertices);
        glDisableVertexAttribArray(locNormals);
        glDisableVertexAttribArray(locUVs);
}
