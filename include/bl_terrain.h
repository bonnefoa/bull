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
                                btTransform _model,
                                const char *_heightmapImage,
                                const char *_textureSetName,
                                std::vector<float> _textureSetHeights
                         ) :
                        model(_model),
                        blTexture(_blTexture),
                        gridWidth(_gridWidth),
                        gridLenght(_gridLenght),
                        heightScale(_heightScale),
                        minHeight(_minHeight),
                        maxHeight(_maxHeight),
                        heightmapImage(_heightmapImage),
                        textureSetName(_textureSetName),
                        textureSetHeights(_textureSetHeights) {};
                ~BlTerrain();

                void init();
                void loadInBuffer();
                void drawElement();

                void bindVertices(GLint locVertices);
                void bindUVTexture(GLint locUVTexture);
                void bindTextures();
                void bindModelMatrix(GLint uniformModel);
                void bindGridSize(GLint locGridLenght, GLint locGridWidth);

                btRigidBody *rigidBody;
                btTransform model;

        private:
                BlTexture *blTexture;
                std::vector <btVector3> vertices;
                std::vector <float> UVs;
                std::vector <unsigned int> indices;

                int gridWidth;
                int gridLenght;
                float heightScale;
                float minHeight;
                float maxHeight;

                const char *heightmapImage;
                char *heightMapData;
                const char *textureSetName;
                std::vector<float> textureSetHeights;

                void createRigidBody();
                char *extractHeightmapData(BlImage *blImage);
                void initTextures();
                void initVertices();
                void initIndices();
                void initUVs();

                GLuint heightmapBuffer;
                GLuint textureBuffer;
                GLuint vertexBuffer;
                GLuint indiceBuffer;
                GLuint uvBuffer;
};

#endif
