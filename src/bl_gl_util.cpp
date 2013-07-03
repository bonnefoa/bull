#include "bl_gl_util.h"

void bindVectors(GLint location, GLuint buffer)
{
        if(location < 0) {
                return;
        }
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glVertexAttribPointer(location, 4 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);
}

void bindUVs(GLint location, GLuint buffer)
{
        if(location < 0) {
                return;
        }
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glVertexAttribPointer(location, 2 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);
}

void drawIndices(GLuint indiceBuffer, int size)
{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, (void *)0);
}


void disableLocation(GLint location)
{
        if(location < 0) {
                return;
        }
        glDisableVertexAttribArray(location);
}

void loadIndicesInBuffer(GLuint buffer, std::vector<unsigned int> &indices)
{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        indices.size() * sizeof(unsigned int),
                        &indices[0], GL_STATIC_DRAW);
}

void loadVectorsInBuffer(GLuint buffer, std::vector<btVector3> &vectors)
{
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER,
                        vectors.size() * sizeof(btVector3),
                        &vectors[0], GL_STATIC_DRAW);
}

void loadUVsInBuffer(GLuint buffer, std::vector<float> &uvs)
{
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER
                        , uvs.size() * sizeof(float)
                        , &uvs[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}
