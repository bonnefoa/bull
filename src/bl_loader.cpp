#include "bl_loader.h"
#include <bl_matrix.h>
#include <bl_physic.h>

btTransform BlLoader::readShapeTransform(YAML::Node node, btVector3 position)
{
        btQuaternion rotation = node["rotation"].as<btQuaternion>();
        return btTransform(rotation, position);
}

btConvexShape *BlLoader::readCollisionShape(YAML::Node node)
{
        std::string shape = node["shape"].as<std::string>();
        btConvexShape *collisionShape = (btConvexShape*)
                malloc(sizeof(btConvexShape));
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

std::map<std::string, btRigidBody*> BlLoader::readShapeNode(YAML::Node shapeNode, btVector3 position)
{
        std::map<std::string, btRigidBody*> mapIndexBody;

        std::string name = shapeNode["name"].as<std::string>();
        float mass = shapeNode["mass"].as<float>();
        btConvexShape *collisionShape = readCollisionShape(shapeNode);
        INFO("Shape of mass %f and type %s\n", mass,
                        collisionShape->getName());
        btTransform transform = readShapeTransform(shapeNode, position);

        btRigidBody *body = buildRigidBody(mass, collisionShape,
                        transform);
        mapIndexBody[name] = body;
        return mapIndexBody;
}

std::map<std::string, btVector3> BlLoader::readShapeOffset(YAML::Node node)
{
        std::map<std::string, btVector3> mapIndexOffset;
        std::string name = node["name"].as<std::string>();
        btVector3 origin = node["origin"].as<btVector3>();
        mapIndexOffset[name] = origin;
        return mapIndexOffset;
}

BlTerrain* BlLoader::loadTerrain(YAML::Node node)
{
        std::string name = node["name"].as<std::string>();
        const char *image = strduplicate(
                        (node["image"].as<std::string>()).c_str());
        btVector3 position = readVector3(node["position"]);
        btVector3 scale = readVector3(node["scale"]);

        int   gridWidth = node["gridWidth"].as<int>();
        int   gridLenght = node["gridLenght"].as<int>();
        float heightScale = node["heightScale"].as<float>();
        float minHeight = node["minHeight"].as<float>();
        float maxHeight = node["maxHeight"].as<float>();

        const char *diffuseTexture = strduplicate((node["diffuseTexture"]
                                .as<std::string>()).c_str());
        const char *normalTexture = strduplicate((node["normalTexture"]
                                .as<std::string>()).c_str());

        BlTerrain *blTerrain = new BlTerrain(blTexture,
                        gridWidth, gridLenght,
                        heightScale,
                        minHeight, maxHeight,
                        position, scale,
                        image,
                        diffuseTexture, normalTexture);
        return blTerrain;
}

std::vector<BlModel*> *BlLoader::loadModel(YAML::Node node)
{
        std::string modelPath = node["filename"].as<std::string>();
        const char *normal = NULL;
        if(node["normal"]) {
                normal = strduplicate((node["normal"].as<std::string>()).c_str());
        }
        const char *diffuse = NULL;
        if(node["diffuse"]) {
                diffuse = strduplicate((node["diffuse"].as<std::string>()).c_str());
        }
        btVector3 position = readVector3(node["position"]);

        std::map<std::string, btRigidBody*> mapIndexBody;
        std::map<std::string, btVector3> mapIndexOffset;
        if(node["shape"]) {
                const char *shapeFile = (node["shape"].as<std::string>()).c_str();
                YAML::Node shapeNode = YAML::LoadFile(shapeFile);
                mapIndexBody = readShapeNode(shapeNode, position);
                mapIndexOffset = readShapeOffset(shapeNode);
        }

        std::vector<BlModel*> * models =
                blMeshLoader.loadModelFile(modelPath.c_str(), position,
                        mapIndexBody, mapIndexOffset, diffuse, normal);
        return models;
}

BlCharacter *BlLoader::loadCharacter(YAML::Node node)
{
        std::vector<BlModel*> *blModels = loadModel(node);
        const char *shapeFile = (node["shape"].as<std::string>()).c_str();
        YAML::Node shapeNode = YAML::LoadFile(shapeFile);
        float mass = shapeNode["mass"].as<float>();
        float linearDamping = node["linearDamping"].as<float>();
        float angularDamping = node["angularDamping"].as<float>();
        float angularThreshold = node["angularThreshold"].as<float>();
        float speed = node["speed"].as<float>();
        btVector3 position = readVector3(node["position"]);
        btTransform transform = readShapeTransform(shapeNode, position);
        btConvexShape *collisionShape = readCollisionShape(shapeNode);
        BlCharacter *blCharacter = new BlCharacter(blModels,
                        mass, linearDamping, angularDamping,
                        angularThreshold,
                        speed,
                        collisionShape,
                        blState, transform);
        return blCharacter;
}

BlLightAmbient *BlLoader::loadAmbientNode(YAML::Node node)
{
        std::string filename = node["filename"].as<std::string>();
        return blMeshLoader.loadAmbientFile(filename.c_str());
}

std::vector<BlLightPoint*> BlLoader::loadLightNode(YAML::Node node)
{
        std::string modelPath = node["filename"].as<std::string>();
        std::vector<BlModel*> *nodeModels = loadModel(node);
        btVector3 position = readVector3(node["position"]);
        return blMeshLoader.loadLightFile(modelPath.c_str(),
                        position, nodeModels);
}

BlScene *BlLoader::loadScene(const char *filename)
{
        std::vector<BlTerrain*> *terrains = new std::vector<BlTerrain*>();
        std::vector<BlModel*> *models = new std::vector<BlModel*>();
        std::vector<BlLightPoint*> *lights = new std::vector<BlLightPoint*>();
        BlLightAmbient* ambient = new BlLightAmbient(btVector3());
        YAML::Node config = YAML::LoadFile(filename);
        YAML::Node lightNodes = config["lights"];
        if(config["ambient"]) {
                ambient = loadAmbientNode(config["ambient"]);
        }
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
                std::vector<BlModel*> *nodeModels = loadModel(*it);
                for (std::vector<BlModel*>::iterator it = nodeModels->begin();
                                it != nodeModels->end(); ++it) {
                        models->push_back(*it);
                }
        }
        YAML::Node configTerrain = config["terrains"];
        for(YAML::const_iterator it=configTerrain.begin();
                        it!=configTerrain.end();++it) {
                BlTerrain *nodeTerrain = loadTerrain(*it);
                terrains->push_back(nodeTerrain);
        }
        YAML::Node characterNode = config["character"];
        BlCharacter *blCharacter = loadCharacter(characterNode);
        INFO("Scene has %i models, %i lights\n", models->size(),
                        lights->size());
        return new BlScene(models, lights, ambient, terrains, blCharacter);
}
