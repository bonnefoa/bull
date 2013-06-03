#include "bl_vertice.h"

void addRectangle(std::vector<btVector3> *vertices,
                btVector3 lowerLeft,
                btVector3 upRight,
                bool zUp)
{
        btVector3 up = btVector3(0, upRight[1], 0);
        btVector3 lower = btVector3(0, lowerLeft[1], 0);
        if(zUp){
                lower = btVector3(0, 0, upRight[2]);
                up = btVector3(0, 0, lowerLeft[2]);
        }
        btVector3 left = btVector3(lowerLeft[0], 0, 0);
        btVector3 right = btVector3(upRight[0], 0, 0);

        vertices->push_back(lower + left);
        vertices->push_back(up + left);
        vertices->push_back(up + right);

        vertices->push_back(lower + left);
        vertices->push_back(up + right);
        vertices->push_back(lower + right);
}
