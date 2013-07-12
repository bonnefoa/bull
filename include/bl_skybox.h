#ifndef BL_SKYBOX_H
#define BL_SKYBOX_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bl_texture.h>

class BlSkybox
{
public:
        BlSkybox( const char *_pathXpos,
                        const char *_pathXneg,
                        const char *_pathYpos,
                        const char *_pathYneg,
                        const char *_pathZpos,
                        const char *_pathZneg);

        ~BlSkybox();

        void init();
        void loadInBuffer();
        void drawSkybox(GLint locModel, GLint locVertices);

private:

        GLuint cubeMapTexture;
        GLuint vertexBuffer;
        GLuint indiceBuffer;

        const char *imagePaths[6];
        static const GLenum targetTypes[6];
        static const GLfloat cubeVertices[24];
        static const GLushort cubeIndices[24];
};

#endif /* end of include guard: BL_SKYBOX_H */
