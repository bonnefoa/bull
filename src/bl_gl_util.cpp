#include "bl_gl_util.h"

void bindVectors(GLint location, GLuint buffer)
{
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glVertexAttribPointer(location, 4 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);
}

void loadVectorsInBuffer(GLuint buffer, std::vector<btVector3> &vectors)
{
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER,
                        vectors.size() * sizeof(btVector3),
                        &vectors[0], GL_STATIC_DRAW);
}
