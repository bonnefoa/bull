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

void BlProgramModel::bindMVP()
{
        btMatrix3x3 model = btMatrix3x3();
        model.setIdentity();
        btTransform M = btTransform();
        M.setIdentity();
        M.setBasis(model);

        sendMatrix(blInput->view, uniformV);
        sendMatrix(M, uniformM);

        btScalar mat[16];
        blInput->projection.getOpenGLMatrix(mat);
        mat[11] = -1.0f;
        glUniformMatrix4fv(uniformP, 1, GL_FALSE, mat);
}

void BlProgramModel::loadModelInBuffer(BlModel *model)
{
        glUseProgram(programId);
        model->loadInBuffer();
}

void BlProgramModel::displayModel(BlModel *model)
{
        glUseProgram(programId);

        model->drawElement(locationVertexPositionWorldspace);
        bindMVP();
}
