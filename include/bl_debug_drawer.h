#ifndef BL_DEBUG_DRAWER_H
#define BL_DEBUG_DRAWER_H

#include <bullet.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>

class BlDebugDrawer : public btIDebugDraw {
        public:
                void init();
                virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);

        private:
                GLuint lineBuffer;
                GLuint colorBuffer;

                std::vector<btVector3> lines;
                std::vector<btVector3> colors;
};

#endif
