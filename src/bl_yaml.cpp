#include "bl_yaml.h"
#include <bl_util.h>

btVector3 readVector3(YAML::Node node)
{
        if(!node) {
                return btVector3();
        }
        return node.as<btVector3>();
}

float getNodeFloat(YAML::Node node, float def)
{
        if(node) {
                return node.as<float>();
        }
        return def;
}

const char *getNodeChar(YAML::Node node, const char *key)
{
        return strduplicate(node[key].as<std::string>().c_str());
}
