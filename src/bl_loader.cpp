#include "bl_loader.h"
#include <bl_log.h>
#include <string.h>

BlModel *loadModelNode(xmlNode *node)
{
        BlModel *blModel;
        blModel = new BlModel((const char *)xmlGetProp(node, (const unsigned char *)"filename"));
        blModel->loadAsset();
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
                if(strcmp((char *)node->name, "model") == 0) {
                        models->push_back(loadModelNode(node));
                }
        }
        return models;
}
