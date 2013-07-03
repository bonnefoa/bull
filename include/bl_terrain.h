#ifndef BL_TERRAIN_H
#define BL_TERRAIN_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <bl_image.h>
#include <bl_texture.h>
#include <bl_util.h>
#include <bullet.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

class BlTerrain
{
        public:
                BlTerrain(
                                BlTexture *_blTexture,
                                int   _gridWidth, int   _gridLenght,
                                float _heightScale,
                                float _minHeight, float _maxHeight,
                                btVector3 _position,
                                btVector3 _scale,
                                const char *_heightMapPath,
                                const char *_normalMapPath,
                                const char *_textureSetName,
                                std::vector<float> _textureSetHeights
                         ) :
                        position(_position),
                        scale(_scale),
                        blTexture(_blTexture),
                        gridWidth(_gridWidth),
                        gridLenght(_gridLenght),
                        heightScale(_heightScale),
                        minHeight(_minHeight),
                        maxHeight(_maxHeight),
                        heightMapPath(_heightMapPath),
                        normalMapPath(_normalMapPath),
                        textureSetName(_textureSetName),
                        textureSetHeights(_textureSetHeights) {};
                ~BlTerrain();

                void init();
                void loadInBuffer();
                void drawElement(GLint locModel,
                                GLint locVertices,
                                GLint locNormal,
                                GLint locTangent,
                                GLint locBitangent,
                                GLint locUVTexture,
                                GLint locUVNormal);

                void bindTextures();
                void bindModelMatrix(GLint uniformModel);

                btRigidBody *rigidBody;
                btVector3 position;
                btVector3 scale;

                std::vector <btVector3> vertices;
                std::vector <btVector3> normals;
                std::vector <btVector3> tangents;
                std::vector <btVector3> binormals;

        private:
                BlTexture *blTexture;
                std::vector <float> textureUVs;
                std::vector <float> normalUVs;
                std::vector <unsigned int> indices;

                int gridWidth;
                int gridLenght;
                float heightScale;
                float minHeight;
                float maxHeight;

                const char *heightMapPath;
                const char *normalMapPath;
                char *heightMapData;
                const char *textureSetName;
                std::vector<float> textureSetHeights;

                GLuint heightmapBuffer;
                GLuint textureBuffer;
                GLuint normalTextureBuffer;

                GLuint vertexBuffer;
                GLuint normalBuffer;
                GLuint tangentBuffer;
                GLuint bitangentBuffer;

                GLuint indiceBuffer;
                GLuint uvTextureBuffer;
                GLuint uvNormalBuffer;

        private:
                void createRigidBody();
                std::vector<btVector3> extractImageData(BlImage *blImage);
                void initTextures();
                void initVertices();
                void initHeightmapData();
                void initIndices();
                void initTangents();
                void initUVs();
                BlImage *createNormalHeightmap(BlImage *blImage);
                std::vector<btVector3> averageVectors(std::vector< std::vector<btVector3> > &source);
};

#endif
