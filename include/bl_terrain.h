#ifndef BL_TERRAIN_H
#define BL_TERRAIN_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <bullet.h>

class BlTerrain
{
        public:
                BlTerrain(unsigned int _verticeNumber,
                          btTransform _model,
                          const char *_image);
                ~BlTerrain();

                void init();
                void loadInBuffer();
                void drawElement();

                void bindVertices(GLint locVertices);
                void bindTextures();
                void bindModelMatrix(GLint uniformModel);
                void bindVerticeNumber(GLint locSizeTerrain);

                std::vector <btVector3> vertices;
                std::vector <unsigned int> indices;
                std::vector <unsigned int> normals;

                btTransform model;
                const char *image;
                unsigned int verticeNumber;

        private:

                GLuint textureBuffer;
                GLuint vertexBuffer;
                GLuint indiceBuffer;
                GLuint normalBuffer;
};

#endif
