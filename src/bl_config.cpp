#include "bl_config.h"
#include <bl_log.h>
#include <bl_matrix.h>
#include <yaml.h>
#include <bl_util.h>

int getKeyFromName(YAML::Node node, const char *def)
{
        char *keyStr = (char *)def;
        if(node) {
                keyStr = (char *)(node.as<std::string>()).c_str();
        }
        int res = SDL_GetKeyFromName(keyStr);
        if (res == SDLK_UNKNOWN) {
                ERROR("Key %s is unknown\n", keyStr);
        }
        return res;
}

float getNodeFloat(YAML::Node node, float def)
{
        if(node) {
                return node.as<float>();
        }
        return def;
}

BlConfig *loadBlConfig(const char *configurationFile)
{
        INFO("Loading configuration from file %s\n", configurationFile);
        YAML::Node config = YAML::LoadFile(configurationFile);

        YAML::Node textNode = config["text"];
        const char *fontPath =
                strduplicate((textNode["font"].as<std::string>()).c_str());
        int fontSize = textNode["size"].as<int>();

        YAML::Node gameNode = config["game"];
        float speed = getNodeFloat(gameNode["speed"], 0.001f);
        float mouseSpeed = getNodeFloat(gameNode["mouseSpeed"], 0.0001f);

        YAML::Node cameraNode = config["camera"];
        float degFov = getNodeFloat(cameraNode["fov"], 45.0f);
        float fov = M_PI * degFov / float(90);
        float aspect = getNodeFloat(cameraNode["aspect"], 4.0f/3.0f);
        float zNear = getNodeFloat(cameraNode["zNear"], 0.1f);
        float zFar = getNodeFloat(cameraNode["zFar"], 100.0f);

        float degLookThreshold = getNodeFloat(cameraNode["lookThreshold"], 180.0f);
        float lookThreshold = M_PI * degLookThreshold / 90.0f;

        YAML::Node keyNode = config["keys"];
        int key_forward = getKeyFromName(keyNode["forward"], "w");
        int key_back = getKeyFromName(keyNode["back"], "s");
        int key_left = getKeyFromName(keyNode["left"], "a");
        int key_right = getKeyFromName(keyNode["right"], "d");
        int key_escape = getKeyFromName(keyNode["escape"], "escape");
        int key_alt_escape = getKeyFromName(keyNode["alt_escape"], "q");
        int key_reload = getKeyFromName(keyNode["reload"], "r");
        int key_light = getKeyFromName(keyNode["light"], "l");
        int key_pause = getKeyFromName(keyNode["pause"], "p");
        int key_debug = getKeyFromName(keyNode["debug"], "?");
        int key_cameraFirstPerson = getKeyFromName(keyNode["cameraFirstPerson"], "f1");
        int key_cameraThirdPerson = getKeyFromName(keyNode["cameraThirdPerson"], "f2");

        YAML::Node networkNode = config["network"];
        const char *host = strduplicate(networkNode["host"].as<std::string>().c_str());
        int port = networkNode["port"].as<int>();

        btTransform projection = computeProjection(fov, aspect, zNear, zFar);

        BlConfig *blConfig = new BlConfig(
                     mouseSpeed,
                     speed,
                     fontPath,
                     fontSize,

                     lookThreshold,

                     projection,

                     key_pause,
                     key_light,
                     key_reload,
                     key_escape,
                     key_alt_escape,
                     key_forward,
                     key_back,
                     key_left,
                     key_right,
                     key_debug,
                     key_cameraFirstPerson,
                     key_cameraThirdPerson,
                     host,
                     port
                     );

        return blConfig;
}

