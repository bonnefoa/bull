#include "bl_light_point.h"

void BlLightPoint::loadInBuffer(GLuint programId)
{
        GLint locConstantAttenuation = glGetUniformLocation(programId,
                   "lightConstantAttenuation");
        GLint locLinearAttenuation = glGetUniformLocation(programId,
                    "lightLinearAttenuation");
        GLint locQuadraticAttenuation = glGetUniformLocation(programId,
                    "lightQuadraticAttenuation");
        GLint locColor = glGetUniformLocation(programId, "lightColor");
        GLint locPosition = glGetUniformLocation(programId, "lightPosition_worldspace");

        glUniform3fv(locPosition, 1, &position[0]);
        glUniform3fv(locColor, 1, &color[0]);
        glUniform1f(locConstantAttenuation, constantAttenuation);
        glUniform1f(locLinearAttenuation, lineraAttenuation);
        glUniform1f(locQuadraticAttenuation, quadraticAttenuation);
}


void drawLight(GLuint programId)
{

}
