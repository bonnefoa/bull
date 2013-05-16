#include "bl_config.h"
#include <iniparser.h>
#include <bl_log.h>

int getKeyFromName(dictionary *ini, const char *key, const char *def)
{
        char *keyStr = iniparser_getstring(ini, key, (char*)def);
        int res = SDL_GetKeyFromName(keyStr);
        if (res == SDLK_UNKNOWN) {
                ERROR("Key %s is unknown\n", keyStr);
        }
        return res;
}

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

        int key_forward = getKeyFromName(ini, "keys:forward", "w");
        int key_back = getKeyFromName(ini, "keys:back", "s");
        int key_left = getKeyFromName(ini, "keys:left", "a");
        int key_right = getKeyFromName(ini, "keys:right", "d");
        int key_escape = getKeyFromName(ini, "keys:escape", "escape");
        int key_alt_escape = getKeyFromName(ini, "keys:alt_escape", "q");
        int key_reload = getKeyFromName(ini, "keys:reload", "r");
        int key_light = getKeyFromName(ini, "keys:light", "l");
        int key_pause = getKeyFromName(ini, "keys:pause", "space");

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
