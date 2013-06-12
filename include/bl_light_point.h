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
                                std::vector<BlModel*> *_blModels
                                ) :
                        position(_position),
                        color(_color),
                        constantAttenuation(_constantAttenuation),
                        lineraAttenuation(_linearAttenuation),
                        quadraticAttenuation(_quadraticAttenuation),
                        blModels(_blModels)
                        {};

                btVector3 position;
                btVector3 color;
                float constantAttenuation;
                float lineraAttenuation;
                float quadraticAttenuation;
                std::vector<BlModel*> *blModels;

                void loadInBuffer(GLuint programId);
                void init();
                void moveLight(btVector3 position, GLuint programId);

                virtual ~BlLightPoint ();
};

#endif
