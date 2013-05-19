#include "bl_loader.h"

namespace YAML {
        template<>
                struct convert<btVector3> {
                        static Node encode(const btVector3& rhs) {
                                Node node;
                                node.push_back(rhs[0]);
                                node.push_back(rhs[1]);
                                node.push_back(rhs[2]);
                                return node;
                        }

                        static bool decode(const Node& node, btVector3& rhs) {
                                if(!node.IsSequence() || node.size() != 3)
                                        return false;
                                rhs[0] = node[0].as<btScalar>();
                                rhs[1] = node[1].as<btScalar>();
                                rhs[2] = node[2].as<btScalar>();
                                return true;
                        }
                };
}

btVector3 readPosition(YAML::Node node)
{
        if(!node) {
                return btVector3();
        }
        return node.as<btVector3>();
}

std::vector<BlModel*> loadModel(YAML::Node node)
{
        std::string modelPath = node["filename"].as<std::string>();
        const char *image = NULL;
        if(node["image"]) {
                image = strduplicate((node["image"].as<std::string>()).c_str());
        }
        btVector3 position = readPosition(node["position"]);
        float mass = 0.0f;
        return loadModelFile(modelPath.c_str(), position,
                        mass, image);
}

BlLightAmbient *loadAmbientNode(YAML::Node node)
{
        std::string filename = node["filename"].as<std::string>();
        return loadAmbientFile(filename.c_str());
}

std::vector<BlLightPoint*> loadLightNode(YAML::Node node)
{
        std::string modelPath = node["filename"].as<std::string>();
        std::vector<BlModel*> nodeModels = loadModel(node);
        btVector3 position = readPosition(node["position"]);
        return loadLightFile(modelPath.c_str(),
                        position, new std::vector<BlModel*>(nodeModels));
}

BlScene *loadScene(const char *filename)
{
        std::vector<BlModel*> *models = new std::vector<BlModel*>();
        std::vector<BlLightPoint*> *lights = new std::vector<BlLightPoint*>();
        BlLightAmbient* ambient = new BlLightAmbient(btVector3());
        YAML::Node config = YAML::LoadFile(filename);
        YAML::Node lightNodes = config["lights"];
        for(YAML::const_iterator it=lightNodes.begin();
                        it!=lightNodes.end(); ++it) {
                std::vector<BlLightPoint*> nodeLights = loadLightNode(*it);
                for (unsigned int i = 0; i < nodeLights.size(); i++) {
                        lights->push_back(nodeLights[i]);
                }
        }
        YAML::Node modelNodex = config["models"];
        for(YAML::const_iterator it=modelNodex.begin();
                        it!=modelNodex.end();++it) {
                std::vector<BlModel*> nodeModels = loadModel(*it);
                for (unsigned int i = 0; i < nodeModels.size(); i++) {
                        models->push_back(nodeModels[i]);
                }
        }
        if(config["ambient"]) {
                ambient = loadAmbientNode(config["ambient"]);
        }
        return new BlScene(models, lights, ambient);
}
