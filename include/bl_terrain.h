#ifndef BL_TERRAIN_H
#define BL_TERRAIN_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <bullet.h>
#include <bl_image.h>
#include <bl_texture.h>

class BlTerrain
{
        public:
                BlTerrain(
                                BlTexture *_blTexture,
                                int   _gridWidth,
                                int   _gridLenght,
                                float _heightScale,
                                float _minHeight,
                                float _maxHeight,
                                btTransform _model,
                                const char *_heightmapImage,
                                const char *_setTextureName,
                                std::vector<float> _textureHeights
                                );
                ~BlTerrain();

                void init();
                void loadInBuffer();
                void drawElement();

                void bindVertices(GLint locVertices);
                void bindUV(GLint locUV);
                void bindTextures();
                void bindModelMatrix(GLint uniformModel);
                void bindGridSize(GLint locGridLenght, GLint locGridWidth);

                BlTexture *blTexture;
                std::vector <btVector3> vertices;
                std::vector <unsigned int> indices;

                int gridWidth;
                int gridLenght;
                float heightScale;
                float minHeight;
                float maxHeight;

                btRigidBody *rigidBody;
                btTransform model;
                const char *heightmapImage;
                char *heightMapData;
                const char *textureSetName;
                std::vector<float> textureSetHeights;

        private:
                void createRigidBody();
                void extractHeightmapData(BlImage *blImage);
                void initVertices();
                void initIndices();
                void initUVs();

                GLuint heightmapBuffer;
                GLuint textureBuffer;
                GLuint vertexBuffer;
                GLuint indiceBuffer;
                GLuint normalBuffer;
};

#endif
