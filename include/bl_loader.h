#ifndef BL_LOADER_H
#define BL_LOADER_H

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <vector>
#include <bl_model.h>

std::vector<BlModel*> *loadXmlScene(const char *filename);
std::vector<BlModel*> loadAssetFile(const char *modelPath,
                btVector3 position, float mass);

#endif
