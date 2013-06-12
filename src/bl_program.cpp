#include "bl_program.h"
#include <stdio.h>
#include <bl_util.h>
#include <bl_log.h>

void BlProgram::loadProgram()
{
        if (programId == 0) {
                programId = glCreateProgram();
                INFO("Initialized program with id %d\n", programId);
        } else {
                for(unsigned int i = 0; i < shaders.size(); i++) {
                        glDetachShader(programId, (*shaders[i]).shaderId);
                }
        }
        GLint programOk;
        for(unsigned int i = 0; i < shaders.size(); i++){
                GLuint shaderId = (*shaders[i]).loadShader();
                if(shaderId==0) {
                        exit(1);
                }
                glAttachShader(programId, shaderId);
        }
        glLinkProgram(programId);

        glGetProgramiv(programId, GL_LINK_STATUS, &programOk);
        if(!programOk)
        {
                INFO("Failed to link shader program:\n");
                showInfoLog(programId, glGetProgramiv, glGetProgramInfoLog);
                glDeleteProgram(programId);
        }
}

BlProgram::~BlProgram(void)
{
        INFO("Deleting program %i\n", programId);
        glDeleteProgram(programId);
        for (std::vector<BlShader*>::iterator it = shaders.begin();
                        it != shaders.end(); ++it) {
                delete (*it);
        }
}
