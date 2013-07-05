#include "bl_terrain.h"
#include <bl_gl_util.h>
#include <bl_matrix.h>
#include <bl_log.h>
#include <bl_heightmap.h>
#include <sys/stat.h>
#include <map>
#include <bl_vertice.h>

void BlTerrain::pushVertice(int x, int z)
{
        float deltaX = (gridWidth - 1) / 2.0f;
        float deltaZ = (gridLenght- 1) / 2.0f;
        int index = x + z * gridWidth;
        unsigned char heightData = heightMapData[index];
        float height = heightData * heightScale;
        btVector3 vert = btVector3(x - deltaX,
                        height, z - deltaZ);
        vertices.push_back(vert);
}

void BlTerrain::pushNormal(int x, int z)
{
        if(x == 0 || x >= gridWidth - 2 || z == 0 || z >= gridLenght - 2) {
                normals.push_back(btVector3(0, 1, 0));
                return;
        }
        float heightLeft =  heightMapData[(x - 1) + z * gridWidth] * heightScale;
        float heightRight = heightMapData[(x + 1) + z * gridWidth] * heightScale;
        float heightUp =    heightMapData[x + (z + 1) * gridWidth] * heightScale;
        float heightDown =  heightMapData[x + (z - 1) * gridWidth] * heightScale;
        btVector3 horizontal(-2, heightLeft - heightRight, 0);
        btVector3 vertical(0, heightUp - heightDown, 2);
        btVector3 normal = horizontal.cross(vertical).normalize();
        normals.push_back(normal);
}

void BlTerrain::initVertices()
{
        vertices = std::vector<btVector3>();
        for(int z = 0; z < gridLenght - 1; z++) {
                for(int x = 0; x < gridWidth - 1; x++) {
                        pushVertice(x, z);
                        pushVertice(x, z + 1);
                        pushVertice(x + 1, z + 1);

                        pushVertice(x, z);
                        pushVertice(x + 1, z + 1);
                        pushVertice(x + 1, z);
                }
        }
}

void BlTerrain::initNormals()
{
        normals = std::vector<btVector3>();
        for(int z = 0; z < gridLenght - 1; z++) {
                for(int x = 0; x < gridWidth - 1; x++) {
                        pushNormal(x, z);
                        pushNormal(x, z + 1);
                        pushNormal(x + 1, z + 1);

                        pushNormal(x, z);
                        pushNormal(x + 1, z + 1);
                        pushNormal(x + 1, z);
                }
        }
}

void BlTerrain::initTangents()
{
        tangents = std::vector<btVector3>();
        bitangents = std::vector<btVector3>();


        std::vector< std::vector<btVector3> >tempTangents(gridWidth * gridLenght);
        std::vector< std::vector<btVector3> >tempBitangents(gridWidth * gridLenght);

        for(unsigned int i = 0; i < indices.size(); i+=3) {
                int ind0 = indices[i];
                int ind1 = indices[i + 1];
                int ind2 = indices[i + 2];
                btVector3 vert0 = vertices[ind0];
                btVector3 vert1 = vertices[ind1];
                btVector3 vert2 = vertices[ind2];
                btVector3 uv0 = btVector3(textureUVs[ind0 * 2 ],
                                textureUVs[ind0 * 2 + 1], 0);
                btVector3 uv1 = btVector3(textureUVs[ind1 * 2 ],
                                textureUVs[ind1 * 2 + 1], 0);
                btVector3 uv2 = btVector3(textureUVs[ind2 * 2 ],
                                textureUVs[ind2 * 2 + 1], 0);
                btVector3 tangent;
                btVector3 bitangent;
                computeTangentSpace(vert0, vert1, vert2,
                                uv0, uv1, uv2,
                                tangent, bitangent);
                for(int j = 0; j < 3; j++) {
                        tempTangents[indices[i + j]].push_back(tangent);
                        tempBitangents[indices[i + j]].push_back(bitangent);
                }
        }
        tangents = averageVectors(tempTangents);
        bitangents = averageVectors(tempBitangents);

        for(unsigned int i = 0; i < indices.size(); i++) {
                btVector3 normal = normals[i];
                btVector3 tangent = tangents[i];
                btVector3 bitangent = bitangents[i];

                tangent = (tangent - normal * normal.dot(tangent)).normalize();

                if(normal.cross(tangent).dot(bitangent) < 0.0f) {
                        tangent = tangent * -1.0f;
                }
        }
}

void BlTerrain::initIndices()
{
        for(unsigned int i = 0; i < vertices.size(); i++) {
                indices.push_back(i);
        }
}

void BlTerrain::initUVs()
{
        for(std::vector<btVector3>::iterator it = vertices.begin();
                        it != vertices.end();
                        it++) {
                btVector3 vert = (*it);
                float u = float(vert.x());
                float v = float(vert.z());
                textureUVs.push_back(u);
                textureUVs.push_back(v);
        }
}

std::vector<btVector3> BlTerrain::averageVectors(std::vector< std::vector<btVector3> > &source)
{
        std::vector<btVector3> means;
        for(std::vector< std::vector<btVector3> >::iterator it = source.begin();
                        it != source.end(); it++) {
                std::vector<btVector3> vectors = *it;
                btVector3 mean;
                for(std::vector<btVector3>::iterator it2 = vectors.begin();
                                it2 != vectors.end(); it2++) {
                        mean += *it2;
                }
                mean /= vectors.size();
                means.push_back(mean.normalize());
        }
        return means;
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

std::vector<btVector3> BlTerrain::extractImageData(BlImage *blImage)
{
        std::vector<btVector3> gridData = std::vector<btVector3>(gridWidth * gridLenght);
        int lengthIncrement = blImage->height / gridLenght;
        int widthIncrement = blImage->width / gridWidth;
        for(int x = 0; x < gridWidth; x++) {
                for(int z = 0; z < gridLenght; z++) {
                        int index = x + z * gridWidth;
                        gridData[index] = blImage->getPixelsAt(
                                        x * widthIncrement,
                                        z * lengthIncrement);
                }
        }
        return gridData;
}

void BlTerrain::initTextures()
{
        diffuseTextureBuffer = blTexture->fetchTexture(diffuseTexturePath);
        normalTextureBuffer = blTexture->fetchTexture(normalTexturePath);
        heightmapTextureBuffer = blTexture->fetchTexture(heightMapPath);
}

void BlTerrain::initHeightmapData()
{
        BlImage *heightMapImage = readPngImage(heightMapPath);

        std::vector<btVector3> heightVectors = extractImageData(heightMapImage);
        heightMapData = (char *)malloc(gridWidth * gridLenght * sizeof(char));
        for(int x = 0; x < gridWidth; x++) {
                for(int z = 0; z < gridLenght; z++) {
                        int index = x + z * gridWidth;
                        heightMapData[index] = heightVectors[index][0] * 255.f;
                }
        }

        delete heightMapImage;
}

void BlTerrain::init()
{
        glGenBuffers(1, &indiceBuffer);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &tangentBuffer);
        glGenBuffers(1, &normalBuffer);
        glGenBuffers(1, &bitangentBuffer);
        glGenBuffers(1, &uvBuffer);

        initHeightmapData();
        createRigidBody();
        initVertices();
        initNormals();
        initIndices();

        initTextures();
        initUVs();
        initTangents();
}

BlTerrain::~BlTerrain()
{
        glDeleteBuffers(1, &indiceBuffer);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &normalBuffer);
        glDeleteBuffers(1, &tangentBuffer);
        glDeleteBuffers(1, &bitangentBuffer);
        glDeleteBuffers(1, &uvBuffer);

        glDeleteTextures(1, &heightmapTextureBuffer);
        glDeleteTextures(1, &diffuseTextureBuffer);
        glDeleteTextures(1, &normalTextureBuffer);
        free(heightMapData);
}

void BlTerrain::loadInBuffer()
{
        loadVectorsInBuffer(vertexBuffer, vertices);
        loadVectorsInBuffer(normalBuffer, normals);
        loadVectorsInBuffer(tangentBuffer, tangents);
        loadVectorsInBuffer(bitangentBuffer, bitangents);

        loadUVsInBuffer(uvBuffer, textureUVs);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER
                        , indices.size() * sizeof(unsigned int)
                        , &indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BlTerrain::bindTextures()
{
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, heightmapTextureBuffer);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, diffuseTextureBuffer);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normalTextureBuffer);
}

void BlTerrain::bindModelMatrix(GLint uniformModel)
{
        btTransform model = buildModelMatrix(scale, position);
        sendTransform(model, uniformModel);
}

void BlTerrain::drawElement(GLint locModel,
                GLint locVertices,
                GLint locNormal,
                GLint locTangent,
                GLint locBitangent,
                GLint locUVTexture) {
        bindModelMatrix(locModel);
        bindVectors(locVertices, vertexBuffer);
        bindVectors(locNormal, normalBuffer);
        bindVectors(locTangent, tangentBuffer);
        bindVectors(locBitangent, bitangentBuffer);
        bindUVs(locUVTexture, uvBuffer);

        bindTextures();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void *)0);

        disableLocation(locUVTexture);
        disableLocation(locVertices);
        disableLocation(locTangent);
        disableLocation(locBitangent);
        disableLocation(locNormal);
}
