#include <bl_shader.h>
#include <bl_file.h>
#include <stdio.h>
#include <stdlib.h>
#include <bl_util.h>

GLuint BlShader::loadShader()
{
        int length;
        GLchar * source = (GLchar *)file_contents(shaderFile, &length);
        GLint shader_ok;
        if (!source) {
                return 0;
        }
        if(shaderId == 0) {
                shaderId = glCreateShader(shaderType);
        }
        glShaderSource(shaderId, 1, (const GLchar **)&source, &length);
        free(source);
        glCompileShader(shaderId);
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shader_ok);
        if (!shader_ok) {
                fprintf(stderr, "Failed to compile %s:\n", shaderFile);
                show_info_log(shaderId, glGetShaderiv, glGetShaderInfoLog);
                glDeleteShader(shaderId);
                return 0;
        }
        fprintf(stdout, "Shader source compiled %s:\n", shaderFile);
        return shaderId;
}
