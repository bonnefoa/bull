#ifndef BL_LIGHT_AMBIENT_H
#define BL_LIGHT_AMBIENT_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bullet.h>

class BlLightAmbient {
        public:
                BlLightAmbient (btVector3 _color) : color(_color) {};

                btVector3 color;

                void loadInBuffer(GLuint uniformAmbientLightColor);
};

#endif
