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
        btMatrix3x3 M = btMatrix3x3();
        M.setIdentity();
        btMatrix3x3 V = blInput->view;
        btMatrix3x3 P = blInput->projection;
        btMatrix3x3 MV = V * M;
        btMatrix3x3 MVP = P * MV;

        btScalar mat[16];
        MVP.getOpenGLSubMatrix(mat);
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, mat);
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
