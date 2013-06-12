#ifndef BL_MODEL_H
#define BL_MODEL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <bullet.h>
#include <bl_uvs.h>
#include <bl_state.h>
#include <bl_texture.h>

class BlModel {
        public:
                BlModel(BlTexture *_blTexture,
                                std::vector< std::vector<BlModel*>* > *_children,
                                std::vector<btVector3> _vertices,
                                std::vector<unsigned int> _indices,
                                std::vector<btVector3> _normals,
                                std::vector<btVector3> _tangents,
                                std::vector<btVector3> _bitangents,
                                std::vector<BlUvs> _blUVs,
                                btVector3 _position,
                                btRigidBody *_rigidBody,

                                const char *_name,
                                const char *_textureFile
                       ) :
                        blTexture(_blTexture),
                        children(_children),
                        vertices(_vertices),
                        indices(_indices),
                        normals(_normals),
                        tangents(_tangents),
                        bitangents(_bitangents),
                        blUVs(_blUVs),
                        position(_position),
                        rigidBody(_rigidBody),
                        name(_name),
                        textureFile(_textureFile)
        { };
                void init();
                void loadInBuffer();

                void drawElement(GLint locModel, GLint locVertices,
                GLint locNormals, GLint locUVS);

                BlTexture *blTexture;
                std::vector< std::vector<BlModel*>* > *children;

                std::vector <btVector3> vertices;
                std::vector <unsigned int> indices;
                std::vector <btVector3> normals;
                std::vector <btVector3> tangents;
                std::vector <btVector3> bitangents;
                std::vector <BlUvs> blUVs;
                btVector3 position;
                btRigidBody *rigidBody;
                const char *name;
                const char *textureFile;

                virtual ~BlModel ();

                GLuint vertexBuffer;
                GLuint indiceBuffer;
                GLuint textureBuffer;

        private:
                GLuint uvBuffer;
                GLuint normalBuffer;
                GLuint tangentBuffer;
                GLuint bitangentBuffer;

                void bindVertices(GLint locationVertex);
                void bindNormals(GLint locNormals);
                void bindUVs(GLint locUVs);
                void bindModelMatrix(GLint uniformM);

};

#endif
