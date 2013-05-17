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
                 virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
                 virtual void reportErrorWarning(const char* warningString);
                 virtual void draw3dText(const btVector3& location,const char* textString);
                 virtual void setDebugMode(int debugMode);
                 virtual int getDebugMode() const;

        private:
                int debugMode;

                GLuint lineBuffer;
                GLuint colorBuffer;

                std::vector<btVector3> lines;
                std::vector<btVector3> colors;
};

#endif
