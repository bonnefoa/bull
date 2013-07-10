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

                                int _width,
                                int _height,

                                btTransform _projection,
                                float _lookThreshold,

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
                                int _key_cameraFirstPerson,
                                int _key_cameraThirdPerson,
                                const char *_host,
                                int _port
                                ) :

                                fontPath(_fontPath),
                                fontSize(_fontSize),

                                width(_width),
                                height(_height),

                                mouseSpeed(_mouseSpeed),
                                speed(_speed),

                                projection(_projection),
                                lookThreshold(_lookThreshold),

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
                                key_cameraFirstPerson(_key_cameraFirstPerson),
                                key_cameraThirdPerson(_key_cameraThirdPerson),
                                host(_host),
                                port(_port),

                                interpupillaryDistance(0.064f)
                {} ;

                const char* fontPath;
                int fontSize;

                int width;
                int height;

                float mouseSpeed;
                float speed;

                btTransform projection;
                float lookThreshold;

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
                int key_cameraFirstPerson;
                int key_cameraThirdPerson;
                const char* host;
                int port;

                float interpupillaryDistance;
};

BlConfig *loadBlConfig(const char *configurationFile);

#endif
