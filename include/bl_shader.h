#ifndef BL_SHADER_H
#define BL_SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>

class BlShader {
        const char * shaderFile;
        const GLenum shaderType;

        public:
                GLuint shaderId;

                BlShader(const char *shaderFile, const GLenum shaderType)
                        : shaderFile(shaderFile), shaderType(shaderType) {shaderId=0;};
                GLuint loadShader(void);
        private:
                void showInfoLog(void);
};

#endif
