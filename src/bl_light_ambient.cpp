#include "bl_light_ambient.h"

void BlLightAmbient::loadInBuffer(GLuint uniformAmbientColor)
{
        glUniform3fv(uniformAmbientColor, 1, &color[0]);
}
