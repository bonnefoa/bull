#ifndef BL_LOADER_H
#define BL_LOADER_H

#include <stdio.h>
#include <string.h>
#include <vector>
#include <bl_model.h>
#include <bl_scene.h>
#include <bl_mesh_loader.h>
#include <yaml.h>

BlScene *loadScene(const char *filename);

#endif
