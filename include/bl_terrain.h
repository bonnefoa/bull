#ifndef BL_TERRAIN_H
#define BL_TERRAIN_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <bullet.h>

class BlTerrain
{
        public:
                BlTerrain(unsigned int vertNumbers,
                          const char *_image);
                ~BlTerrain();

                void init();
                void loadInBuffer();
                void drawElement();

                std::vector <btVector3> vertices;
                std::vector <unsigned int> indices;
                std::vector <unsigned int> normals;

                const char *image;
        private:

                GLuint vertexBuffer;
                GLuint indiceBuffer;
                GLuint normalBuffer;
                GLuint textureBuffer;
};

#endif
