#include "bl_program_model.h"
#include <bl_util.h>
#include <bl_light_point.h>
#include <bullet.h>
#include <bl_log.h>
#include <bl_matrix.h>

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

void BlProgramModel::bindModelMatrix(BlModel *blModel)
{
        glUseProgram(programId);

        btRigidBody *body = blModel->rigidBody;
        btTransform trans;
        trans.setIdentity();
        if(body && body->getMotionState()) {
                body->getMotionState()->getWorldTransform(trans);
        } else {
                trans.setOrigin(blModel->position);
        }
        sendTransform(trans, uniformM);
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
        glUseProgram(programId);

        bindModelMatrix(blModel);
        sendTransform(blInput->view, uniformV);

        blModel->bindVertices(locVertices);
        blModel->bindNormals(locNormals);
        blModel->bindUVs(locUVs);
        blModel->drawElement();

        glDisableVertexAttribArray(locVertices);
        glDisableVertexAttribArray(locNormals);
        glDisableVertexAttribArray(locUVs);
}
