#include "bl_config.h"
#include <iniparser.h>
#include <bl_log.h>

BlConfig *loadBlConfig(const char *configurationFile)
{
        INFO("Loading configuration from file %s\n", configurationFile);
        dictionary *ini = iniparser_load(configurationFile);

        float speed = iniparser_getdouble(ini, "game:speed", 0.001f);
        float mouseSpeed = iniparser_getdouble(ini, "game:mouseSpeed", 0.0001f);

        float degFov = iniparser_getdouble(ini, "camera:fov", 45);
        float fov = M_PI * degFov / float(90);
        float aspect = iniparser_getdouble(ini, "camera:aspect", 4.0f/3.0f);
        float zNear = iniparser_getdouble(ini, "camera:zNear", 0.1f);
        float zFar = iniparser_getdouble(ini, "camera:zFar", 100.0f);

        int key_forward = SDLK_w;
        int key_back = SDLK_s;
        int key_left = SDLK_a;
        int key_right = SDLK_d;
        int key_escape = SDLK_ESCAPE;
        int key_alt_escape = SDLK_q;
        int key_reload = SDLK_r;
        int key_light = SDLK_l;
        int key_pause = SDLK_SPACE;

        BlConfig *blConfig = new BlConfig(mouseSpeed,
                     speed,

                     fov,
                     aspect,
                     zNear,
                     zFar,

                     key_pause,
                     key_light,
                     key_reload,
                     key_escape,
                     key_alt_escape,
                     key_forward,
                     key_back,
                     key_left,
                     key_right);

        iniparser_freedict(ini);
        return blConfig;
}
