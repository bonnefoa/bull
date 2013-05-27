#include "bl_terrain.h"
#include <bl_matrix.h>
#include <bl_log.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <map>

BlTerrain::BlTerrain(BlTexture *_blTexture,
                int   _gridWidth, int   _gridLenght,
                float _heightScale,
                float _minHeight, float _maxHeight,
                btTransform _model,
                const char *_heightmapImage,
                const char *_textureSetName,
                std::vector<float> _textureSetHeights
                )
        : blTexture(_blTexture),
                gridWidth(_gridWidth),
                gridLenght(_gridLenght),
                heightScale(_heightScale),
                minHeight(_minHeight),
                maxHeight(_maxHeight),
                model(_model),
                heightmapImage(_heightmapImage),
                textureSetName(_textureSetName),
                textureSetHeights(_textureSetHeights)
{
        float deltaX = (gridWidth + 1) / 2;
        float deltaZ = (gridLenght + 1) / 2;
        for(int z = 0; z < gridLenght; z++) {
                for(int x = 0; x < gridWidth; x++) {
                        btVector3 vert = btVector3(x - deltaX, 0, z - deltaZ);
                        vertices.push_back(vert);
                }
        }
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

void BlTerrain::extractHeightmapData(BlImage *blImage)
{
        size_t size = gridWidth * gridLenght * sizeof(char);
        heightMapData = (char *)malloc(size);
        int lengthIncrement = blImage->height / gridLenght;
        int widthIncrement = blImage->width / gridWidth;
        for(int x = 0; x < gridWidth; x++) {
                for(int z = 0; z < gridLenght; z++) {
                        int index = x + z * gridWidth;
                        int pixelIndex =
                                x * blImage->numChannels * widthIncrement
                                + z * blImage->width * blImage->numChannels
                                    * lengthIncrement;
                        heightMapData[index] =
                                blImage->pixels[pixelIndex];
                }
        }
}

void BlTerrain::init()
{
        glGenBuffers(1, &indiceBuffer);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &normalBuffer);

        INFO("Fetching texture set %s\n", textureSetName);
        textureBuffer = blTexture->fetchTexture(textureSetName);
        INFO("Fetching heightmap image %s\n", heightmapImage);
        heightmapBuffer = blTexture->fetchTexture(heightmapImage);
        BlImage *blImage = readPngImage(heightmapImage);
        extractHeightmapData(blImage);
        createRigidBody();
        delete blImage;

}

BlTerrain::~BlTerrain()
{
        glDeleteBuffers(1, &indiceBuffer);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &normalBuffer);
        if(heightmapBuffer > 0)
                glDeleteTextures(1, &heightmapBuffer);
}

void BlTerrain::loadInBuffer()
{
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                        vertices.size() * sizeof(btVector3),
                        &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER
                        , normals.size() * sizeof(btVector3)
                        , &normals[0], GL_STATIC_DRAW);

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
