#include "bl_program_model.h"
#include "btBulletDynamicsCommon.h"

void BlProgramModel::init()
{
        glUseProgram(programId);

        uniformMVP = glGetUniformLocation(programId, "MVP");
        locationVertexPositionWorldspace = glGetAttribLocation(programId, "vertexPosition_modelspace");

        glUseProgram(0);
}

void BlProgramModel::bindMVP()
{
        btTransform M = btTransform();
        M.setIdentity();
        btTransform V = blInput->view;
        btTransform P = blInput->projection;
        btTransform MV = V * M;
        btTransform MVP = P * MV;

        btScalar mat[16];
        MVP.getOpenGLMatrix(mat);
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, mat);
}

void BlProgramModel::loadModelInBuffer(BlModel *model)
{
        glUseProgram(programId);
        model->loadInBuffer();
        glUseProgram(0);
}

void BlProgramModel::displayModel(BlModel *model)
{
        glUseProgram(programId);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        model->drawElement(locationVertexPositionWorldspace);
        bindMVP();

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glUseProgram(0);
}
