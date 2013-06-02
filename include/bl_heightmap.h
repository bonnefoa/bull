#ifndef BL_HEIGHTMAP_H
#define BL_HEIGHTMAP_H

#include <bl_image.h>
#include <bullet.h>
#include <bl_log.h>
#include <vector>

BlImage *generateNormalMapFromHeightmap(BlImage *blImage);
std::vector <btVector3> generateOffGrid(BlImage *heightmap, int gridWidth
                , int gridLenght);
int getDeltaPixelSum(int pointX, int pointY, int width,
                int height, BlImage *blImage);

#endif
