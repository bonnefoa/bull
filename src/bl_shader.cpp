#include <bl_shader.h>
#include <bl_file.h>
#include <stdio.h>
#include <stdlib.h>

void BlShader::showInfoLog()
{
        GLint log_length;
        char *log;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &log_length);
        log = (char *) malloc(log_length);
        glGetShaderInfoLog(shaderId, log_length, NULL, log);
        fprintf(stderr, "%s", log);
        free(log);
}

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
                showInfoLog();
                glDeleteShader(shaderId);
                return 0;
        }
        fprintf(stdout, "Shader source compiled %s:\n", shaderFile);
        return shaderId;
}
