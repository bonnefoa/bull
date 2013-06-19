#include "bl_program_texture.h"

BlProgramTexture *getProgramTexture()
{
        std::vector<BlShader*> shaders;
        BlShader *vertexShader = new BlShader("glsl/texture.vert"
                        , GL_VERTEX_SHADER);
        BlShader *fragmentShader = new BlShader("glsl/texture.frag"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(vertexShader);
        shaders.push_back(fragmentShader);

        BlProgramTexture *blProgramTexture = new BlProgramTexture(shaders);
        blProgramTexture->loadProgram();
        blProgramTexture->init();
        return blProgramTexture;
}

void BlProgramTexture::init()
{
        glUseProgram(programId);

        glGenBuffers(1, &quadVertexBuffer);
        static const GLfloat quadBufferData[] = {
                -1.0f, -1.0f,  0.0f,
                 1.0f, -1.0f,  0.0f,
                1.0f,  1.0f,  0.0f,

                -1.0f, -1.0f,  0.0f,
                 1.0f,  1.0f,  0.0f,
                -1.0f,  1.0f,  0.0f,
        };

        locVertices = glGetAttribLocation(programId, "vertexPosition");
        samplerTexture = glGetUniformLocation(programId, "samplerTexture");
        glUniform1i(samplerTexture, 4);

        glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadBufferData)
                        , quadBufferData, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BlProgramTexture::displayTexture(GLuint textureId)
{
        glViewport(0, 0, 512, 512);
        glUseProgram(programId);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glEnableVertexAttribArray(locVertices);
        glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
        glVertexAttribPointer(locVertices, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(locVertices);
        glUseProgram(0);
}
