#ifndef BL_MODEL_H
#define BL_MODEL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <bullet.h>
#include <bl_uvs.h>

class BlModel {
        public:
                BlModel(std::vector<btVector3> verticesInput
                                , std::vector<unsigned int> indicesInput
                                , std::vector<BlUvs> uvsInput
                                , btVector3 positionInput
                                , float massInput
                                , const char *nameInput) :
                        vertices(verticesInput)
                        , indices(indicesInput)
                        , uvs(uvsInput)
                        , position(positionInput)
                        , mass(massInput)
                        , name(nameInput) {};
                void init();
                void loadInBuffer();
                bool loadAsset(void);
                void drawElement(GLuint locationVertex);
                void clear(void);

                std::vector <btVector3> vertices;
                std::vector <unsigned int> indices;
                std::vector <BlUvs> uvs;
                btVector3 position;
                float mass;
                const char *name;
                btRigidBody *rigidBody;

        private:
                GLuint vertexBuffer;
                GLuint indiceBuffer;
};

#endif
