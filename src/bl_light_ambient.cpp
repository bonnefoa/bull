#include "bl_light_ambient.h"

void BlLightAmbient::loadInBuffer(GLuint programId)
{
        glUseProgram(programId);
        GLuint uniformAmbientColor = glGetUniformLocation(programId, "ambientColor");
        glUniform3fv(uniformAmbientColor, 1, &color[0]);
}
