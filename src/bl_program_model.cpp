#include "bl_program_model.h"
#include <bl_util.h>
#include <bl_light_point.h>
#include <bullet.h>
#include <bl_log.h>

void BlProgramModel::init()
{
        glUseProgram(programId);

        uniformM = glGetUniformLocation(programId, "M");
        uniformV = glGetUniformLocation(programId, "V");
        uniformP = glGetUniformLocation(programId, "P");
        uniformLightColor = glGetUniformLocation(programId
                        , "lightColor");
        uniformLightPosition = glGetUniformLocation(programId
                        , "lightPosition_modelspace");
        locVertexPosModelspace = glGetAttribLocation(programId
                        , "vertexPosition_modelspace");
        locNormalModelspace = glGetAttribLocation(programId
                        , "vertexNormal_modelspace");
        locUV = glGetAttribLocation(programId
                        , "vertexUV");
        INFO("vertex location %i\n", locVertexPosModelspace);
        INFO("normal location %i\n", locNormalModelspace);
        INFO("uv location %i\n", locUV);
        if(locNormalModelspace < 0 || locUV < 0 || locVertexPosModelspace < 0){
                ERROR("A location is unused");
        }
}

void sendMatrix(btTransform trans, GLuint uniform)
{
        btScalar mat[16];
        trans.getOpenGLMatrix(mat);
        glUniformMatrix4fv(uniform, 1, GL_FALSE, mat);
}

void BlProgramModel::bindModelMatrix(BlModel *blModel)
{
        btRigidBody *body = blModel->rigidBody;
        btTransform trans;
        trans.setIdentity();
        if(body && body->getMotionState()) {
                body->getMotionState()->getWorldTransform(trans);
        }
        sendMatrix(trans, uniformM);
}

void BlProgramModel::bindProjectionMatrix()
{
        btScalar mat[16];
        blInput->projection.getOpenGLMatrix(mat);
        mat[11] = -1.0f;
        glUniformMatrix4fv(uniformP, 1, GL_FALSE, mat);
}

void BlProgramModel::displayModel(BlModel *blModel)
{
        glUseProgram(programId);
        bindModelMatrix(blModel);
        sendMatrix(blInput->view, uniformV);
        blModel->drawElement(locVertexPosModelspace, locUV,
                        locNormalModelspace);
}
