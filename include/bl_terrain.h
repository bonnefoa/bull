#ifndef BL_TERRAIN_H
#define BL_TERRAIN_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <bullet.h>
#include <bl_image.h>

class BlTerrain
{
        public:
                BlTerrain(
                                int   _gridWidth,
                                int   _gridLenght,
                                float _heightScale,
                                float _minHeight,
                                float _maxHeight,
                                btTransform _model,
                                const char *_image);
                ~BlTerrain();

                void init();
                void loadInBuffer();
                void drawElement();

                void bindVertices(GLint locVertices);
                void bindTextures();
                void bindModelMatrix(GLint uniformModel);
                void bindGridSize(GLint locGridLenght, GLint locGridWidth);

                std::vector <btVector3> vertices;
                std::vector <unsigned int> indices;
                std::vector <unsigned int> normals;

                int gridWidth;
                int gridLenght;
                float heightScale;
                float minHeight;
                float maxHeight;

                btRigidBody *rigidBody;
                btTransform model;
                const char *image;
                BlImage *blImage;

        private:
                void createRigidBody();

                GLuint textureBuffer;
                GLuint vertexBuffer;
                GLuint indiceBuffer;
                GLuint normalBuffer;
};

#endif
