#include "bl_light_point.h"

void BlLightPoint::loadInBuffer(GLuint uniformLightPosition, GLuint uniformLightColor)
{
        glUniform3fv(uniformLightPosition, 1, &position[0]);
        glUniform3fv(uniformLightColor, 1, &color[0]);
}
