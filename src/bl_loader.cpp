#include "bl_loader.h"
#include <bl_log.h>
#include <string.h>

BlModel *loadModelNode(xmlNode *node)
{
        xmlNode *attrNode;
        BlModel *blModel;
        xmlAttr *attr = node->properties;
        for(attrNode = attr->children;
                        attrNode;
                        attrNode = attrNode->next) {
                if(strcmp((char *)attrNode->name, "filename") == 0) {
                         blModel = new BlModel(
                                         (const char *)attrNode->content);
                }
        }
        return blModel;
}

std::vector<BlModel*> *loadScene(char *filename)
{
        xmlDoc         *document;
        xmlNode        *root, *node;

        std::vector<BlModel*> *models = new std::vector<BlModel*>();

        document = xmlReadFile(filename, NULL, 0);
        root = xmlDocGetRootElement(document);
        for (node = root->children; node; node = node->next) {
                INFO("\t Child is <%s> (%i) %s\n",
                                node->name, node->type, node->content);
                if(strcmp((char *)node->name, "model") == 0) {
                        models->push_back(loadModelNode(node));
                }
        }
        return models;
}
