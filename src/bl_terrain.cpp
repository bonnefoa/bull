#include "bl_terrain.h"
#include <bl_gl_util.h>
#include <bl_matrix.h>
#include <bl_log.h>
#include <bl_heightmap.h>
#include <sys/stat.h>
#include <map>

void BlTerrain::initVertices()
{
        float deltaX = (gridWidth - 1) / 2.0f;
        float deltaZ = (gridLenght- 1) / 2.0f;
        for(int z = 0; z < gridLenght; z++) {
                for(int x = 0; x < gridWidth; x++) {
                        int index = x + z * gridWidth;
                        unsigned char heightData = heightMapData[index];
                        float height = heightData * heightScale;
                        btVector3 vert = btVector3(x - deltaX,
                                        height, z - deltaZ);
                        vertices.push_back(vert);
                }
        }
}

void BlTerrain::initIndices()
{
        int maxZ = gridWidth * (gridLenght - 1);
        for(int z = 0; z < maxZ; z+=gridWidth) {
                for(int x = z; x < z + gridWidth - 1; x++) {
                        indices.push_back(x);
                        indices.push_back(x + gridWidth);
                        indices.push_back(x + 1);
                        indices.push_back(x + 1);
                        indices.push_back(x + gridWidth);
                        indices.push_back(x + gridWidth + 1);
                }
        }
}

void BlTerrain::initUVs()
{
        for (std::vector<btVector3>::iterator
                        it = vertices.begin();
                        it != vertices.end(); ++it) {
                float u = (*it)[0] / 32.0f;
                float v = (*it)[2] / 32.0f;
                UVs.push_back(u);
                UVs.push_back(v);
        }
}

void BlTerrain::createRigidBody()
{
        int upAxis = 1;
        INFO("Constructing terrain shape with gridWith %i, gridLenght %i, heightScale %f, minHeight %f, maxHeight %f\n",
                        gridWidth, gridLenght, heightScale,
                        minHeight, maxHeight);
        btHeightfieldTerrainShape *shape =
                new btHeightfieldTerrainShape(
                                gridWidth,
                                gridLenght,
                                heightMapData,
                                heightScale,
                                minHeight,
                                maxHeight,
                                upAxis,
                                PHY_UCHAR,
                                false
                                );
        shape->setLocalScaling(scale);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f
                        , NULL, shape, btVector3(0,0,0) );
        rigidBody = new btRigidBody(rbInfo);
        btTransform model = buildModelMatrix(btVector3(1,1,1), position);
        rigidBody->setWorldTransform(model);
}

char *BlTerrain::extractHeightmapData(BlImage *blImage)
{
        size_t size = gridWidth * gridLenght * sizeof(char);
        char *height = (char *)malloc(size);
        int lengthIncrement = blImage->height / gridLenght;
        int widthIncrement = blImage->width / gridWidth;
        for(int x = 0; x < gridWidth; x++) {
                for(int z = 0; z < gridLenght; z++) {
                        int index = x + z * gridWidth;
                        height[index] =
                                blImage->getPixelAt(x * widthIncrement,
                                                z * lengthIncrement);
                }
        }
        return height;
}

void BlTerrain::createNormalHeightmap(BlImage *blImage)
{
        struct stat st;
        if(stat(normalmapImage, &st) != 0) {
                BlImage *normalImage = generateNormalMapFromHeightmap(blImage);
                normalImage->writeImage(normalmapImage);
                delete blImage;
                delete normalImage;
        }
}

void BlTerrain::initTextures()
{
        textureBuffer = blTexture->fetchTexture(textureSetName);
        heightmapBuffer = blTexture->fetchTexture(heightmapImage);
        normalBuffer = blTexture->fetchTexture(normalmapImage);
}

void BlTerrain::init()
{
        glGenBuffers(1, &indiceBuffer);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &tangentBuffer);
        glGenBuffers(1, &cotangentBuffer);
        glGenBuffers(1, &uvBuffer);

        BlImage *blImage = readPngImage(heightmapImage);
        heightMapData = extractHeightmapData(blImage);
        createNormalHeightmap(blImage);

        createRigidBody();
        initVertices();
        initIndices();

        initTextures();
        initUVs();

        delete blImage;
}

BlTerrain::~BlTerrain()
{
        glDeleteBuffers(1, &indiceBuffer);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &tangentBuffer);
        glDeleteBuffers(1, &cotangentBuffer);
        glDeleteBuffers(1, &uvBuffer);
        if(heightmapBuffer > 0)
                glDeleteTextures(1, &heightmapBuffer);
        if(normalBuffer > 0)
                glDeleteTextures(1, &normalBuffer);
        free(heightMapData);
}

void BlTerrain::loadInBuffer()
{
        loadVectorsInBuffer(vertexBuffer, vertices);
        loadVectorsInBuffer(tangentBuffer, tangents);
        loadVectorsInBuffer(cotangentBuffer, cotangents);

        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glBufferData(GL_ARRAY_BUFFER
                        , UVs.size() * sizeof(float)
                        , &UVs[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER
                        , indices.size() * sizeof(unsigned int)
                        , &indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BlTerrain::bindUVTexture(GLint locUVTexture)
{
        glEnableVertexAttribArray(locUVTexture);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(locUVTexture, 2 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);
}

void BlTerrain::bindTextures()
{
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, heightmapBuffer);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normalBuffer);
}

void BlTerrain::bindModelMatrix(GLint uniformModel)
{
        btTransform model = buildModelMatrix(scale, position);
        sendTransform(model, uniformModel);
}

void BlTerrain::drawElement(GLint locModel,
                GLint locVertices,
                GLint locTangent,
                GLint locCotangent,
                GLint locUVTexture) {
        bindModelMatrix(locModel);
        bindVectors(locVertices, vertexBuffer);
        bindVectors(locTangent, tangentBuffer);
        bindVectors(locCotangent, cotangentBuffer);
        bindUVTexture(locUVTexture);
        bindTextures();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void *)0);

        glDisableVertexAttribArray(locUVTexture);
        glDisableVertexAttribArray(locVertices);
}
