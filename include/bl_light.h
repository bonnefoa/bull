#ifndef BL_LIGHT_H
#define BL_LIGHT_H

#include <bullet.h>

class BlLight {
        public:
                BlLight (btVector3 _position,
                                btVector3 _color) :
                        position(_position), color(_color) {};
                virtual ~BlLight ();

                btVector3 position;
                btVector3 color;
};

#endif
