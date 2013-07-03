#ifndef BL_VERTICE_H
#define BL_VERTICE_H

#include <bullet.h>
#include <vector>

void addRectangle(std::vector<btVector3> *vertices,
                btVector3 lowerLeft,
                btVector3 upRight);
void add2dRectangle(std::vector<float> *vertices,
                const float x, const float y,
                const float width, const float height);
void addRectangleUV(std::vector<float> *uvs);

void computeTangentSpace(btVector3 &vert1,
                btVector3 &vert2,
                btVector3 &vert3,
                btVector3 &uv1,
                btVector3 &uv2,
                btVector3 &uv3,
                btVector3 &bitangent,
                btVector3 &tangent);

#endif
