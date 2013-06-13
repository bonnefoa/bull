#ifndef BL_CONFIG_H
#define BL_CONFIG_H

#include <SDL.h>
#include <bullet.h>

class BlConfig {
        public:
                BlConfig(
                                float _mouseSpeed,
                                float _speed,
                                const char* _fontPath,
                                int _fontSize,

                                btTransform _projection,

                                int _key_pause,
                                int _key_light,
                                int _key_reload,
                                int _key_escape,
                                int _key_alt_escape,
                                int _key_forward,
                                int _key_back,
                                int _key_left,
                                int _key_right,
                                int _key_debug,
                                const char *_host,
                                int _port
                                ) :

                                fontPath(_fontPath),
                                fontSize(_fontSize),
                                mouseSpeed(_mouseSpeed),
                                speed(_speed),
                                projection(_projection),
                                key_pause(_key_pause),
                                key_light(_key_light),
                                key_reload(_key_reload),
                                key_escape(_key_escape),
                                key_alt_escape(_key_alt_escape),
                                key_forward(_key_forward),
                                key_back(_key_back),
                                key_left(_key_left),
                                key_right(_key_right),
                                key_debug(_key_debug),
                                host(_host),
                                port(_port)
                {} ;

                const char* fontPath;
                int fontSize;
                float mouseSpeed;
                float speed;

                btTransform projection;

                int key_pause;
                int key_light;
                int key_reload;
                int key_escape;
                int key_alt_escape;
                int key_forward;
                int key_back;
                int key_left;
                int key_right;

                int key_debug;
                const char* host;
                int port;
};

BlConfig *loadBlConfig(const char *configurationFile);

#endif
