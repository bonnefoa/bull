#ifndef BL_YAML_H
#define BL_YAML_H

#include <bullet.h>
#include <yaml.h>

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

        template<>
                struct convert<std::vector<float> > {
                        static Node encode(const std::vector<float>& rhs) {
                                Node node;
                                std::vector<float>::const_iterator it;
                                for (it = rhs.begin();it != rhs.end(); ++it) {
                                        node.push_back(*it);
                                }
                                return node;
                        }

                        static bool decode(const Node& node,
                                        std::vector<float>& rhs) {
                                if(!node.IsSequence())
                                        return false;
                                YAML::const_iterator it;
                                for(it=node.begin();
                                                it!=node.end();
                                                ++it) {
                                        rhs.push_back(it->as<float>());
                                }
                                return true;
                        }
                };

        template<>
                struct convert<std::vector<std::string> > {
                        static Node encode(
                                        const std::vector<std::string>& rhs) {
                                Node node;
                                std::vector<std::string>::const_iterator it;
                                for (it = rhs.begin();it != rhs.end(); ++it) {
                                        node.push_back(*it);
                                }
                                return node;
                        }

                        static bool decode(const Node& node,
                                        std::vector<std::string>& rhs) {
                                if(!node.IsSequence())
                                        return false;
                                YAML::const_iterator it;
                                for(it=node.begin();
                                                it!=node.end();
                                                ++it) {
                                        rhs.push_back(it->as<std::string>());
                                }
                                return true;
                        }
                };

}

btVector3 readVector3(YAML::Node node);
float getNodeFloat(YAML::Node node, float def);
const char *getNodeChar(YAML::Node node, const char *key);

#endif /* end of include guard: BL_YAML_H */
