#ifndef BL_LIGHT_H
#define BL_LIGHT_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bullet.h>

class BlLight {
        public:
                BlLight (btVector3 _position,
                                btVector3 _color) :
                        position(_position), color(_color) {};
                virtual ~BlLight ();

                btVector3 position;
                btVector3 color;

                void loadInBuffer(GLuint uniformLightPosition,
                                GLuint uniformLightPositionLightColor);
};

#endif
