#ifndef BL_LIGHT_H
#define BL_LIGHT_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bullet.h>
#include <vector>
#include <bl_model.h>

class BlLightPoint {
        public:
                BlLightPoint (btVector3 _position,
                                btVector3 _color,
                                float _constantAttenuation,
                                float _linearAttenuation,
                                float _quadraticAttenuation,
                                std::vector<BlModel*> *_models
                                ) :
                        position(_position),
                        color(_color),
                        constantAttenuation(_constantAttenuation),
                        lineraAttenuation(_linearAttenuation),
                        quadraticAttenuation(_quadraticAttenuation),
                        models(_models)
                        {};

                btVector3 position;
                btVector3 color;
                float constantAttenuation;
                float lineraAttenuation;
                float quadraticAttenuation;
                std::vector<BlModel*> *models;

                void loadInBuffer(GLuint programId);
};

#endif
