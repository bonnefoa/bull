#include "bl_terrain.h"
#include <bl_gl_util.h>
#include <bl_matrix.h>
#include <bl_log.h>
#include <bl_heightmap.h>
#include <sys/stat.h>
#include <map>
#include <bl_vertice.h>

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
        for(int z = 0; z < gridLenght; z++) {
                for(int x = 0; x < gridWidth; x++) {
                        float u = float(x) / float(gridWidth);
                        float v = float(z) / float(gridLenght);
                        textureUVs.push_back(u);
                        textureUVs.push_back(v);

                        float normalU = float(x) / float(gridWidth);
                        float normalV = float(z) / float(gridLenght);
                        normalUVs.push_back(normalU);
                        normalUVs.push_back(normalV);
                }
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
                means.push_back(mean);
        }
        return means;
}

void BlTerrain::initTangents()
{
        std::vector< std::vector<btVector3> > vertTangents
                = std::vector< std::vector<btVector3> >(vertices.size());
        std::vector< std::vector<btVector3> > vertNormals
                = std::vector< std::vector<btVector3> >(vertices.size());
        std::vector< std::vector<btVector3> > vertBinormals
                = std::vector< std::vector<btVector3> >(vertices.size());
        for (unsigned int i = 0; i < indices.size(); i+=3) {
                unsigned int ind1 = indices[i];
                unsigned int ind2 = indices[i + 1];
                unsigned int ind3 = indices[i + 2];
                btVector3 &vert1 = vertices[ind1];
                btVector3 &vert2 = vertices[ind2];
                btVector3 &vert3 = vertices[ind3];
                btVector3 uv1 = btVector3(textureUVs[ind1 * 2 ],
                                textureUVs[ind1 * 2 + 1], 0);
                btVector3 uv2 = btVector3(textureUVs[ind2 * 2 ],
                                textureUVs[ind2 * 2 + 1], 0);
                btVector3 uv3 = btVector3(textureUVs[ind3 * 2 ],
                                textureUVs[ind3 * 2 + 1], 0);

                btVector3 normal;
                btVector3 tangent;
                btVector3 binormal;

                computeTangentSpace(vert1, vert2, vert3,
                                uv1, uv2, uv3,
                                normal, binormal, tangent);

                for(unsigned int j = i; j < i + 3; j++) {
                        int indice = indices[j];
                        vertTangents[indice].push_back(tangent);
                        vertNormals[indice].push_back(normal);
                        vertBinormals[indice].push_back(binormal);
                }
        }
        normals = averageVectors(vertNormals);
        tangents = averageVectors(vertTangents);
        binormals = averageVectors(vertBinormals);
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

BlImage *BlTerrain::createNormalHeightmap(BlImage *blImage)
{
        struct stat st;
        if(stat(normalMapPath, &st) != 0) {
                BlImage *normalImage = generateNormalMapFromHeightmap(blImage);
                normalImage->writeImage(normalMapPath);
                return normalImage;
        }
        return readPngImage(normalMapPath);
}

void BlTerrain::initTextures()
{
        textureBuffer = blTexture->fetchTexture(textureSetName);
        heightmapBuffer = blTexture->fetchTexture(heightMapPath);
        normalBuffer = blTexture->fetchTexture(normalMapPath);
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
        glGenBuffers(1, &cotangentBuffer);
        glGenBuffers(1, &uvTextureBuffer);
        glGenBuffers(1, &uvNormalBuffer);

        initHeightmapData();
        createRigidBody();
        initVertices();
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
        glDeleteBuffers(1, &cotangentBuffer);
        glDeleteBuffers(1, &uvTextureBuffer);
        glDeleteBuffers(1, &uvNormalBuffer);
        if(heightmapBuffer > 0)
                glDeleteTextures(1, &heightmapBuffer);
        if(normalTextureBuffer > 0)
                glDeleteTextures(1, &normalTextureBuffer);
        free(heightMapData);
}

void BlTerrain::loadInBuffer()
{
        loadVectorsInBuffer(vertexBuffer, vertices);
        loadVectorsInBuffer(normalBuffer, normals);
        loadVectorsInBuffer(tangentBuffer, tangents);
        loadVectorsInBuffer(cotangentBuffer, binormals);

        loadUVsInBuffer(uvTextureBuffer, textureUVs);
        loadUVsInBuffer(uvNormalBuffer, normalUVs);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER
                        , indices.size() * sizeof(unsigned int)
                        , &indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BlTerrain::bindTextures()
{
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, heightmapBuffer);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureBuffer);
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
                GLint locCotangent,
                GLint locUVTexture,
                GLint locUVNormal) {
        bindModelMatrix(locModel);
        bindVectors(locVertices, vertexBuffer);
        bindVectors(locNormal, normalBuffer);
        bindVectors(locTangent, tangentBuffer);
        bindVectors(locCotangent, cotangentBuffer);
        bindUVs(locUVTexture, uvTextureBuffer);
        bindUVs(locUVNormal, uvNormalBuffer);

        bindTextures();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void *)0);

        glDisableVertexAttribArray(locUVNormal);
        glDisableVertexAttribArray(locUVTexture);
        glDisableVertexAttribArray(locVertices);
        glDisableVertexAttribArray(locTangent);
        glDisableVertexAttribArray(locCotangent);
        glDisableVertexAttribArray(locNormal);
}
