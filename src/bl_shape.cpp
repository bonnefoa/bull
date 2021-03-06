#include "bl_shape.h"
#include <bl_log.h>
#include <bl_util.h>

btCollisionShape *guessShape(BlModel *blModel)
{
        std::vector<btVector3> vertices = blModel->vertices;
        btVector3 maxPoint = btVector3(0.0f, 0.0f, 0.0f);
        btCollisionShape *shape = (btCollisionShape*) malloc(sizeof(btCollisionShape));
        for (std::vector<btVector3>::iterator it = vertices.begin();
                        it != vertices.end(); ++it) {
                btVector3 point = *it;
                btScalar x = point.getX();
                btScalar y = point.getY();
                btScalar z = point.getZ();
                setIfMax(x, maxPoint.getX(), maxPoint, &btVector3::setX);
                setIfMax(y, maxPoint.getY(), maxPoint, &btVector3::setY);
                setIfMax(z, maxPoint.getZ(), maxPoint, &btVector3::setZ);
        }
        shape = new btBoxShape(maxPoint);
        INFO("Guessed box shape half extent %f %f %f\n",
                        maxPoint[0], maxPoint[1], maxPoint[2]);
        return shape;
}

//void drawShape(BlModel *blModel)
//{
        //GLuint shapeBuffer;
        //glGenBuffers(1, &shapeBuffer);
        //glBindBuffer(GL_ARRAY_BUFFER, shapeBuffer);
        //glDeleteBuffers(1, &shapeBuffer);
//}
