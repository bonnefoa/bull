#ifndef BL_LIGHT_H
#define BL_LIGHT_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bullet.h>

class BlLightPoint {
        public:
                BlLightPoint (btVector3 _position,
                                btVector3 _color) :
                        position(_position), color(_color)
                        {};

                btVector3 position;
                btVector3 color;

                void loadInBuffer(GLuint uniformLightPosition,
                                GLuint uniformLightColor);
};

#endif
