#ifndef BL_LOADER_H
#define BL_LOADER_H

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <vector>
#include <bl_model.h>

std::vector<BlModel*> *loadScene(char *filename);

#endif
