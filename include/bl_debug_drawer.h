#ifndef BL_DEBUG_DRAWER_H
#define BL_DEBUG_DRAWER_H

#include <bullet.h>
#include <GL/glew.h>
#include <GL/gl.h>

class BlDebugDrawer : public btIDebugDraw {
        public:
                void init();

        private:
                GLuint lineBuffer;
                std::vector<btVector3> lines;
};

#endif
