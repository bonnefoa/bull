#include "bl_program_model.h"
#include <bl_util.h>
#include <bullet.h>
#include <bl_log.h>

void BlProgramModel::init()
{
        glUseProgram(programId);

        uniformMVP = glGetUniformLocation(programId, "MVP");
        uniformM = glGetUniformLocation(programId, "M");
        uniformV = glGetUniformLocation(programId, "V");
        uniformP = glGetUniformLocation(programId, "P");
        locationVertexPositionWorldspace = glGetAttribLocation(programId
                        , "vertexPosition_modelspace");
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

void BlProgramModel::loadModelInBuffer(BlModel *model)
{
        glUseProgram(programId);
        model->loadInBuffer();
        bindProjectionMatrix();
}

void BlProgramModel::displayModel(BlModel *blModel)
{
        glUseProgram(programId);
        bindModelMatrix(blModel);
        sendMatrix(blInput->view, uniformV);
        blModel->drawElement(locationVertexPositionWorldspace);
}
