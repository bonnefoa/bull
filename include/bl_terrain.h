#ifndef BL_TERRAIN_H
#define BL_TERRAIN_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <bullet.h>

class BlTerrain
{
        public:
                BlTerrain(unsigned int size,
                          btVector3 _position,
                          const char *_image);
                ~BlTerrain();

                void init();
                void loadInBuffer();
                void drawElement();

                void bindVertices(GLint locVertices);
                void bindTextures();
                void bindModelMatrix(GLint uniformM);

                std::vector <btVector3> vertices;
                std::vector <unsigned int> indices;
                std::vector <unsigned int> normals;

                btVector3 position;
                const char *image;
        private:

                GLuint vertexBuffer;
                GLuint indiceBuffer;
                GLuint normalBuffer;
                GLuint textureBuffer;
};

#endif
