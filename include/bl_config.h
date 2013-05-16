#ifndef BL_CONFIG_H
#define BL_CONFIG_H

#include <SDL.h>

class BlConfig {
        public:
                BlConfig(
                                float _mouseSpeed,
                                float _speed,

                                float _fov,
                                float _aspect,
                                float _zNear,
                                float _zFar,

                                int _key_pause,
                                int _key_light,
                                int _key_reload,
                                int _key_escape,
                                int _key_alt_escape,
                                int _key_forward,
                                int _key_back,
                                int _key_left,
                                int _key_right) :

                                mouseSpeed(_mouseSpeed),
                                speed(_speed),
                                fov(_fov),
                                aspect(_aspect),
                                zNear(_zNear),
                                zFar(_zFar),
                                key_pause(_key_pause),
                                key_light(_key_light),
                                key_reload(_key_reload),
                                key_escape(_key_escape),
                                key_alt_escape(_key_alt_escape),
                                key_forward(_key_forward),
                                key_back(_key_back),
                                key_left(_key_left),
                                key_right(_key_right)
                {} ;

                float mouseSpeed;
                float speed;

                float fov;
                float aspect;
                float zNear;
                float zFar;

                int key_pause;
                int key_light;
                int key_reload;
                int key_escape;
                int key_alt_escape;
                int key_forward;
                int key_back;
                int key_left;
                int key_right;
};

BlConfig *loadBlConfig(const char *configurationFile);


#endif
