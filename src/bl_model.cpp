#include "bl_model.h"
#include <bl_log.h>

void BlModel::init()
{
        glGenBuffers(1, &indiceBuffer);
        glGenBuffers(1, &vertexBuffer);
}

void BlModel::clear(void)
{
        glDeleteBuffers(1, &indiceBuffer);
        glDeleteBuffers(1, &vertexBuffer);
}

void BlModel::loadInBuffer()
{
        std::vector <btVector3> relocatedVertices = std::vector <btVector3>();
        for (std::vector<btVector3>::iterator it = vertices.begin();
                        it != vertices.end(); ++it) {
                btVector3 reorderedVertices = btVector3(-(*it).getY(),
                                (*it).getZ(), -(*it).getX());
                relocatedVertices.push_back(reorderedVertices + position);
        }
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                        relocatedVertices.size() * sizeof(btVector3),
                        &relocatedVertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER
                        , indices.size() * sizeof(unsigned int)
                        , &indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void BlModel::drawElement(GLuint locationVertex)
{
        glEnableVertexAttribArray(locationVertex);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(locationVertex, 4 , GL_FLOAT
                        , GL_FALSE, 0, (void *)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void *)0);

        glDisableVertexAttribArray(locationVertex);
}
