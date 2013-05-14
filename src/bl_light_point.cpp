#include "bl_light_point.h"

void BlLightPoint::init()
{
        for (std::vector<BlModel*>::iterator it = blModels->begin();
                        it != blModels->end(); ++it) {
                (*it)->init();
        }
}

void BlLightPoint::moveLight(btVector3 newPosition, GLuint programId)
{
        position = newPosition;
        loadInBuffer(programId);
        for (std::vector<BlModel*>::iterator
                        it = blModels->begin();
                        it != blModels->end(); ++it) {
                (*it)->position = newPosition;
        }
}

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

        for (std::vector<BlModel*>::iterator it = blModels->begin();
                        it != blModels->end(); ++it) {
                (*it)->loadInBuffer();
        }
}

BlLightPoint::~BlLightPoint()
{
        for (std::vector<BlModel*>::iterator it = blModels->begin();
                        it != blModels->end(); ++it) {
                delete (*it);
        }
        delete blModels;
}
