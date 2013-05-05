#include "bl_program_model.h"
#include <bl_util.h>
#include <bullet.h>

void BlProgramModel::init()
{
        glUseProgram(programId);

        uniformMVP = glGetUniformLocation(programId, "MVP");
        locationVertexPositionWorldspace = glGetAttribLocation(programId
                        , "vertexPosition_modelspace");
}

void BlProgramModel::bindMVP()
{
        btTransform M = btTransform();
        M.setIdentity();
        btTransform V = blInput->view;
        printf("V\n");
        printBtTransform(&V);
        btTransform P = blInput->projection;
        printf("P\n");
        printBtTransform(&P);
        btTransform MV = V * M;
        btTransform MVP = P * MV;
        printf("MVP\n");
        printBtTransform(&MVP);

        btScalar mat[16];
        MVP.getOpenGLMatrix(mat);
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, mat);
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
