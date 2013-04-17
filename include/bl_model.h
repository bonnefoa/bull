#ifndef BL_MODEL_H
#define BL_MODEL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include "btBulletDynamicsCommon.h"

class BlModel {

        const char *modelPath;

        public:
                BlModel(const char * modelPath)
                        : modelPath(modelPath) {};
                void init();
                void loadInBuffer();
                bool loadAsset(void);
                void drawElement(GLuint locationVertex);

        private:

                GLuint vertexBuffer;
                GLuint indiceBuffer;

                std::vector <btVector3> vertices;
                std::vector <unsigned int> indices;
};

#endif
