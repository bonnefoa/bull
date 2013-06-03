#include "bl_terrain.h"
#include <bl_matrix.h>
#include <bl_log.h>
#include <bl_heightmap.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
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
        for(unsigned int i =0; i < vertices.size(); i++) {
                indices.push_back(i);
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
        float mass = 0.0f;
        btDefaultMotionState *motionState = NULL;
        btVector3 localInertia(0,0,0);

        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass
                        , motionState, shape, localInertia);
        rigidBody = new btRigidBody(rbInfo);
        rigidBody->proceedToTransform(model);
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
                        int pixelIndex =
                                x * blImage->numChannels * widthIncrement
                                + z * blImage->width * blImage->numChannels
                                    * lengthIncrement;
                        height[index] =
                                blImage->pixels[pixelIndex];
                }
        }
        return height;
}

void BlTerrain::initTextures()
{
        textureBuffer = blTexture->fetchTexture(textureSetName);
        heightmapBuffer = blTexture->fetchTexture(heightmapImage);
}

void BlTerrain::init()
{
        glGenBuffers(1, &indiceBuffer);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &uvBuffer);

        BlImage *blImage = readPngImage(heightmapImage);
        heightMapData = extractHeightmapData(blImage);

        createRigidBody();
        vertices = generateOffGrid(blImage,
                        gridWidth, gridLenght);
        initIndices();

        initTextures();
        initUVs();

        delete blImage;
}

BlTerrain::~BlTerrain()
{
        glDeleteBuffers(1, &indiceBuffer);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &uvBuffer);
        if(heightmapBuffer > 0)
                glDeleteTextures(1, &heightmapBuffer);
}

void BlTerrain::loadInBuffer()
{
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                        vertices.size() * sizeof(btVector3),
                        &vertices[0], GL_STATIC_DRAW);

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

void BlTerrain::bindVertices(GLint locVertices)
{
        glEnableVertexAttribArray(locVertices);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(locVertices, 4 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);
}

void BlTerrain::bindUVTexture(GLint locUVTexture)
{
        glEnableVertexAttribArray(locUVTexture);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(locUVTexture, 2 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);
}

void BlTerrain::bindGridSize(GLint locGridLenght, GLint locGridWidth)
{
        glUniform1i(locGridWidth, gridWidth);
        glUniform1i(locGridLenght, gridLenght);
}

void BlTerrain::bindTextures()
{
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, heightmapBuffer);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);
}

void BlTerrain::bindModelMatrix(GLint uniformModel)
{
        sendTransform(model, uniformModel);
}

void BlTerrain::drawElement() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void *)0);
}
