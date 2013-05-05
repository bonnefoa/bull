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
                glAttachShader(programId, shaderId);
        }
        glLinkProgram(programId);

        glGetProgramiv(programId, GL_LINK_STATUS, &programOk);
        if(!programOk)
        {
                ERROR("Failed to link shader program:\n");
                show_info_log(programId, glGetProgramiv, glGetProgramInfoLog);
                glDeleteProgram(programId);
        }
}
