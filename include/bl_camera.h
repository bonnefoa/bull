#ifndef BL_CAMERA_H
#define BL_CAMERA_H

#include <bullet.h>
#include <bl_config.h>
#include <bl_state.h>

class BlCamera
{
        public:
                BlCamera(BlConfig *_blConfig,
                                BlState *_blState) :
                        direction(btVector3(0,0,-1)),
                        rightDirection(btVector3(1,0,0)),
                        upDirection(btVector3(0,1,0)),
                        rotation(0,0,0,1),
                        blConfig(_blConfig),
                        blState(_blState),
                        position(btVector3(0,0,0))
                                {};
                ~BlCamera();

                void moveCamera(btVector3 position);
                void computeNewCamera();

                btVector3 direction;
                btVector3 rightDirection;
                btVector3 upDirection;
                btTransform view;
                btQuaternion rotation;
        private:
                BlConfig    *blConfig;
                BlState     *blState;
                btVector3 position;

                void computeDirectionFromDelta(float &deltaUp,
                                float &deltaRight);
};

#endif
