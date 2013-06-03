#ifndef BL_VERTICE_H
#define BL_VERTICE_H

#include <bullet.h>
#include <vector>

void addRectangle(std::vector<btVector3> *vertices,
                btVector3 lowerLeft,
                btVector3 upRight,
                bool zUp);

#endif
