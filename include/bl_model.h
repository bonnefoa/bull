#ifndef BL_MODEL_H
#define BL_MODEL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <bullet.h>
#include <bl_uvs.h>

class BlModel {
        public:
                BlModel(std::vector<btVector3> _vertices
                                , std::vector<unsigned int> _indices
                                , std::vector<btVector3> _normals
                                , std::vector<BlUvs> _blUVs
                                , btVector3 _position
                                , float _mass
                                , const char *_name
                                , const char *_image) :
                        vertices(_vertices)
                        , indices(_indices)
                        , normals(_normals)
                        , blUVs(_blUVs)
                        , position(_position)
                        , mass(_mass)
                        , name(_name)
                        , image(_image) {};
                void init();
                void loadInBuffer();
                bool loadAsset(void);
                void drawElement(GLuint locationVertex, GLuint locationUv);
                void clear(void);

                std::vector <btVector3> vertices;
                std::vector <unsigned int> indices;
                std::vector <btVector3> normals;
                std::vector <BlUvs> blUVs;
                btVector3 position;
                float mass;
                const char *name;
                const char *image;
                btRigidBody *rigidBody;

        private:
                GLuint vertexBuffer;
                GLuint indiceBuffer;
                GLuint uvBuffer;
                GLuint textureBuffer;
};

#endif
