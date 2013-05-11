#include "bl_light.h"

void BlLight::loadInBuffer(GLuint uniformLightPosition, GLuint uniformLightColor)
{
        glUniform3fv(uniformLightPosition, 1, &position[0]);
        glUniform3fv(uniformLightColor, 1, &position[0]);
}
