#include "bl_vertice.h"

void addRectangleUV(std::vector<float> *uvs)
{
        uvs->push_back(0.0f);
        uvs->push_back(0.0f);

        uvs->push_back(1.0f);
        uvs->push_back(1.0f);

        uvs->push_back(0.0f);
        uvs->push_back(1.0f);

        uvs->push_back(0.0f);
        uvs->push_back(0.0f);

        uvs->push_back(1.0f);
        uvs->push_back(0.0f);

        uvs->push_back(1.0f);
        uvs->push_back(1.0f);
}

void addRectangle(std::vector<btVector3> *vertices,
                btVector3 lowerLeft,
                btVector3 upRight)
{
        btVector3 up = btVector3(0, upRight[1], 0);
        btVector3 lower = btVector3(0, lowerLeft[1], 0);
        btVector3 left = btVector3(lowerLeft[0], 0, 0);
        btVector3 right = btVector3(upRight[0], 0, 0);

        vertices->push_back(lower + left);
        vertices->push_back(up + right);
        vertices->push_back(up + left);

        vertices->push_back(lower + left);
        vertices->push_back(lower + right);
        vertices->push_back(up + right);
}

void add2dRectangle(std::vector<float> *vertices,
                const float x, const float y,
                const float width, const float height)
{
        vertices->push_back(x);
        vertices->push_back(y);

        vertices->push_back(x + width);
        vertices->push_back(y + height);

        vertices->push_back(x);
        vertices->push_back(y + height);


        vertices->push_back(x);
        vertices->push_back(y);

        vertices->push_back(x + width);
        vertices->push_back(y);

        vertices->push_back(x + width);
        vertices->push_back(y + height);
}

void computeTangentSpace(btVector3 &vert1,
                btVector3 &vert2,
                btVector3 &vert3,
                btVector3 &uv1,
                btVector3 &uv2,
                btVector3 &uv3,
                btVector3 &normal,
                btVector3 &binormal,
                btVector3 &tangent)
{
        btVector3 deltaUV1 = uv2 - uv1;
        btVector3 deltaUV2 = uv3 - uv1;
        (void) deltaUV2;

        btVector3 deltaPos1 = vert2 - vert1;
        btVector3 deltaPos2 = vert3 - vert1;

        normal = deltaPos1.cross(deltaPos2).normalize();
        tangent = (deltaPos1 / deltaUV1[0]).normalize();
        binormal = tangent.cross(normal);
}
