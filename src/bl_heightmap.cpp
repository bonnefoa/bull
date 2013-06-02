#include "bl_heightmap.h"
#include <bl_util.h>

BlImage *generateNormalMapFromHeightmap(BlImage *blImage)
{
        int srcChannels = blImage->numChannels;
        unsigned width = blImage->width;
        unsigned height = blImage->height;
        size_t pixelSize = width * height * RGB_CHANNEL;
        unsigned char *pixels =
                (unsigned char *) malloc(pixelSize);
        memset(pixels, 128, pixelSize);

        int srcLine = width * srcChannels;
        int curLine = width * RGB_CHANNEL;

        for(unsigned int x = 0; x < width; x++) {
                for(unsigned int y = 0; y < height; y++) {
                        int indexSrc = x * srcChannels
                                + y * srcLine;
                        int index = x * RGB_CHANNEL + y * curLine;

                        if(y % (height - 1) == 0 || x % (width - 1) == 0) {
                                pixels[index+2] = 255;
                                continue;
                        }

                        int heightYUp = blImage->pixels[indexSrc + srcLine];
                        int heightYDown = blImage->pixels[indexSrc - srcLine];
                        int heightXLeft = blImage->pixels[indexSrc - srcChannels];
                        int heightXRight = blImage->pixels[indexSrc + srcChannels];
                        btVector3 vecX = btVector3(1,
                                        0, heightXRight - heightXLeft);
                        btVector3 vecY = btVector3(0,
                                        1, heightYUp - heightYDown);
                        btVector3 res = vecX.cross(vecY);
                        res = res.normalize();
                        res = res + btVector3(1.0f,1.0f,1.0f);
                        res = res / btVector3(2.0f, 2.0f, 2.0f);

                        pixels[index] = res[0] * 255;
                        pixels[index+ 1] = res[1] * 255;
                        pixels[index+ 2] = res[2] * 255;
                }
        }
        return new BlImage(width, height, pixels, GL_RGB, RGB_CHANNEL);
}

int getDeltaPixelSum(int pointX, int pointY, int gridWidth,
                int gridHeight, BlImage *blImage)
{
        int xOffset = blImage->width / (2 * gridWidth);
        int yOffset = blImage->height / (2 * gridHeight);
        int startX = pointX - xOffset;
        int startY = pointY - yOffset;
        int endX = pointX + xOffset;
        int endY = pointY + yOffset;
        int sum = 0;

        for(int x = startX; x < endX; x++) {
                for(int y = startY; y  < endY; y++) {
                        int curr = blImage->getPixelAt(x, y);
                        int up = blImage->getPixelAt(x, y+1);
                        int down = blImage->getPixelAt(x, y-1);
                        int left = blImage->getPixelAt(x-1, y);
                        int right = blImage->getPixelAt(x+1, y);
                        sum += abs(up - curr) + abs(down - curr) + abs(left - curr) + abs(right - curr);
                }
        }
        return sum;
}

std::vector <btVector3> generateOffGrid(BlImage *heightmap, int gridWidth
                , int gridLenght)
{
        std::vector<btVector3> gravityPoints;

        int lengthIncrement = heightmap->height / gridLenght;
        int widthIncrement = heightmap->width / gridWidth;
        for(int z = 1; z < gridLenght - 1; z++) {
                for(int x = 1; x < gridWidth - 1; x++) {
                        int pointX = x * widthIncrement;
                        int pointY = z * lengthIncrement;
                        float pixSum = getDeltaPixelSum(pointX, pointY,
                                        gridWidth, gridLenght, heightmap);
                        pixSum /= gridWidth + gridLenght;
                        if(pixSum > 10) {
                                gravityPoints.push_back(btVector3(x, 0, z));
                        }
                }
        }

        float deltaX = (gridWidth - 1) / 2.0f;
        float deltaZ = (gridLenght- 1) / 2.0f;
        float vertPositions[gridWidth][gridLenght][2];

        for(int z = 0; z < gridLenght; z++) {
                for(int x = 0; x < gridWidth; x++) {
                        float vertX = (x - deltaX);
                        float vertZ = (z - deltaZ);
                        vertPositions[x][z][0] = vertX;
                        vertPositions[x][z][1] = vertZ;
                }
        }

        for(int z = 1; z < gridLenght - 1; z++) {
                for(int x = 1; x < (gridWidth - 1); x++) {
                        float vertX = vertPositions[x][z][0];
                        float vertZ = vertPositions[x][z][1];
                        btVector3 res = btVector3(vertX, 0, vertZ);
                        btVector3 gridPoint = btVector3(vertX, 0, vertZ);

                        for (std::vector<btVector3>::iterator it = gravityPoints.begin();
                                        it != gravityPoints.end(); ++it) {
                                btVector3 gravityPoint = *it;
                                btVector3 direction = (gravityPoint - gridPoint).normalize();
                                float distance = gravityPoint.distance(gridPoint);
                                res += direction / distance;
                        }
                        vertPositions[x][z][0] = res[0];
                }
        }

        std::vector <btVector3> vertices;
        for(int z = 0; z < gridLenght; z++) {
                for(int x = 0; x < gridWidth; x++) {
                        btVector3 vert = btVector3(vertPositions[x][z][0], 0,
                                        vertPositions[x][z][1]);
                        vertices.push_back(vert);
                }
        }

        return vertices;
}
