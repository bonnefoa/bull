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

void computeTangentSpace(btVector3 &vert0,
                btVector3 &vert1,
                btVector3 &vert2,
                btVector3 &uv0,
                btVector3 &uv1,
                btVector3 &uv2,
                btVector3 &normal,
                btVector3 &bitangent,
                btVector3 &tangent)
{
        btVector3 deltaPos1 = vert1 - vert0;
        btVector3 deltaPos2 = vert2 - vert0;

        float s1 = uv1[0] - uv0[0];
        float s2 = uv2[0] - uv0[0];

        float t1 = uv1[1] - uv0[1];
        float t2 = uv2[1] - uv0[1];

        float div = 1 / (s1 * t2 - s2 * t1);

        normal = deltaPos1.cross(deltaPos2);
        tangent = (t2 * deltaPos1 -  t1 * deltaPos2) * div;
        bitangent = (-s2 * deltaPos1 + s1 * deltaPos2) * div;
}
