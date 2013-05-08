#ifndef BL_MODEL_H
#define BL_MODEL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <bullet.h>

class BlModel {
        public:
                BlModel(std::vector<btVector3> vertices
                                , std::vector<unsigned int> indices
                                , btVector3 position
                                , float mass) :
                        vertices(vertices)
                        , indices(indices)
                        , position(position)
                        , mass(mass) {};
                void init();
                void loadInBuffer();
                bool loadAsset(void);
                void drawElement(GLuint locationVertex);
                void clear(void);

                std::vector <btVector3> vertices;
                std::vector <unsigned int> indices;
                btVector3 position;
                float mass;

        private:
                GLuint vertexBuffer;
                GLuint indiceBuffer;
};

#endif
