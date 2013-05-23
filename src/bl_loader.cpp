#include "bl_loader.h"
#include <bl_matrix.h>

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

        template<>
                struct convert<btQuaternion> {
                        static Node encode(const btQuaternion& rhs) {
                                Node node;
                                node.push_back(rhs[0]);
                                node.push_back(rhs[1]);
                                node.push_back(rhs[2]);
                                node.push_back(rhs[3]);
                                return node;
                        }

                        static bool decode(const Node& node,
                                        btQuaternion& rhs) {
                                if(!node.IsSequence() || node.size() != 4)
                                        return false;
                                rhs[0] = node[0].as<btScalar>();
                                rhs[1] = node[1].as<btScalar>();
                                rhs[2] = node[2].as<btScalar>();
                                rhs[3] = node[3].as<btScalar>();
                                return true;
                        }
                };
}

btVector3 readVector3(YAML::Node node)
{
        if(!node) {
                return btVector3();
        }
        return node.as<btVector3>();
}

btTransform readShapeTransform(YAML::Node node, btVector3 position)
{
        btQuaternion rotation = node["rotation"].as<btQuaternion>();
        btVector3 origin = node["origin"].as<btVector3>();
        return btTransform(rotation, origin + position);
}

btCollisionShape *readCollisionShape(YAML::Node node)
{
        std::string shape = node["shape"].as<std::string>();
        btCollisionShape *collisionShape = (btCollisionShape*)
                malloc(sizeof(btCollisionShape));
        if(shape == "BOX") {
                btVector3 halfExtents = node["half-extents"]
                        .as<btVector3>();
                collisionShape = new btBoxShape(halfExtents);
        }
        if(shape == "CONE") {
                float radius = node["radius"].as<float>();
                float height = node["height"].as<float>();
                collisionShape = new btConeShape(radius, height);
        } else if(shape == "CAPSULE") {
                float radius = node["radius"].as<float>();
                float height = node["height"].as<float>();
                collisionShape = new btCapsuleShape(radius, height);
        } else if(shape == "SPHERE") {
                float radius = node["radius"].as<float>();
                collisionShape = new btSphereShape(radius);
        } else if(shape == "CYLINDER") {
                btVector3 halfExtents = node["half-extents"]
                        .as<btVector3>();
                collisionShape = new btCylinderShape(halfExtents);
        } else if(shape == "CONVEX_HULL") {
                collisionShape = new btConvexHullShape();
        }
        float margin = 0.0f;
        if(node["margin"]) {
                margin = node["margin"].as<float>();
        }
        collisionShape->setMargin(margin);
        INFO("Collision shape of type %s, margin %f\n",
                        collisionShape->getName(),
                        collisionShape->getMargin());
        return collisionShape;
}

btRigidBody *readShapeNode(YAML::Node node, btVector3 position)
{
        if(!node["shape"]) { return NULL; }
        const char *shapeFile = (node["shape"].as<std::string>()).c_str();
        YAML::Node shapeNode = YAML::LoadFile(shapeFile);

        float mass = shapeNode["mass"].as<float>();
        bool isDynamic = (mass != 0.f);
        btCollisionShape *collisionShape = readCollisionShape(shapeNode);
        INFO("Shape of mass %f and type %s\n", mass,
                        collisionShape->getName());
        btTransform transform = readShapeTransform(shapeNode, position);

        btVector3 localInertia(0,0,0);
        btDefaultMotionState *motionState = NULL;
        if(isDynamic) {
                collisionShape->calculateLocalInertia(mass, localInertia);
                motionState = new btDefaultMotionState(transform);
        }
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass
                        , motionState, collisionShape, localInertia);
        btRigidBody *body = new btRigidBody(rbInfo);
        if(!isDynamic) {
                body->proceedToTransform(transform);
        }
        return body;
}

BlTerrain* loadTerrain(YAML::Node node)
{
        std::string name = node["name"].as<std::string>();
        const char *image = strduplicate(
                        (node["image"].as<std::string>()).c_str());
        btVector3 position = readVector3(node["position"]);
        btVector3 scale = readVector3(node["scale"]);
        btTransform model = buildModelMatrix(scale, position);

        int   gridWidth = node["gridWidth"].as<int>();
        int   gridLenght = node["gridLenght"].as<int>();
        float heightScale = node["heightScale"].as<float>();
        float minHeight = node["minHeight"].as<float>();
        float maxHeight = node["maxHeight"].as<float>();

        BlTerrain *blTerrain = new BlTerrain(
                        gridWidth, gridLenght,
                        heightScale,
                        minHeight, maxHeight,
                        model, image);
        return blTerrain;
}

std::vector<BlModel*> loadModel(YAML::Node node)
{
        std::string modelPath = node["filename"].as<std::string>();
        const char *image = NULL;
        if(node["image"]) {
                image = strduplicate((node["image"].as<std::string>()).c_str());
        }
        btVector3 position = readVector3(node["position"]);
        btRigidBody *rigidBody = readShapeNode(node, position);
        return loadModelFile(modelPath.c_str(), position,
                        rigidBody, image);
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
        btVector3 position = readVector3(node["position"]);
        return loadLightFile(modelPath.c_str(),
                        position, new std::vector<BlModel*>(nodeModels));
}

BlScene *loadScene(const char *filename)
{
        std::vector<BlTerrain*> *terrains = new std::vector<BlTerrain*>();
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
        YAML::Node configModel = config["models"];
        for(YAML::const_iterator it=configModel.begin();
                        it!=configModel.end();++it) {
                std::vector<BlModel*> nodeModels = loadModel(*it);
                for (unsigned int i = 0; i < nodeModels.size(); i++) {
                        models->push_back(nodeModels[i]);
                }
        }
        YAML::Node configTerrain = config["terrains"];
        for(YAML::const_iterator it=configTerrain.begin();
                        it!=configTerrain.end();++it) {
                BlTerrain *nodeTerrain = loadTerrain(*it);
                terrains->push_back(nodeTerrain);
        }
        if(config["ambient"]) {
                ambient = loadAmbientNode(config["ambient"]);
        }
        return new BlScene(models, lights, ambient, terrains);
}
