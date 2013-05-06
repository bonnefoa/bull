#ifndef BL_MODEL_H
#define BL_MODEL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <bullet.h>

class BlModel {
        const char *modelPath;

        public:
                BlModel(const char *modelPath)
                        : modelPath(modelPath) {};
                BlModel(std::vector<btVector3> vertices
                                , std::vector<unsigned int> indices) :
                        vertices(vertices)
                        , indices(indices) {};
                void init();
                void loadInBuffer();
                bool loadAsset(void);
                void drawElement(GLuint locationVertex);

                std::vector <btVector3> vertices;
                std::vector <unsigned int> indices;
        private:
                GLuint vertexBuffer;
                GLuint indiceBuffer;
};

#endif
