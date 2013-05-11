#ifndef BL_LOADER_H
#define BL_LOADER_H

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <vector>
#include <bl_model.h>
#include <bl_scene.h>

BlScene *loadXmlScene(const char *filename);
std::vector<BlModel*> loadModelFile(const char *modelPath,
                btVector3 position=btVector3(), float mass=0.0f);

#endif
