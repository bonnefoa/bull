#ifndef BL_DEBUG_DRAWER_H
#define BL_DEBUG_DRAWER_H

#include <bullet.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <bl_program_debug.h>
#include <bl_state.h>

class BlDebugDrawer : public btIDebugDraw {
        public:
                BlDebugDrawer(BlProgramDebug *_blProgramDebug, BlState *_blState)
                        : blProgramDebug(_blProgramDebug), blState(_blState) {} ;
                void init();
                virtual void drawLine(const btVector3& from,
                                const btVector3& to,const btVector3& color);
                virtual void drawContactPoint(const btVector3& PointOnB,
                                const btVector3& normalOnB,btScalar distance,
                                int lifeTime,const btVector3& color);
                virtual void reportErrorWarning(const char* warningString);
                virtual void draw3dText(const btVector3& location,const char* textString);
                virtual void setDebugMode(int debugMode);
                virtual int getDebugMode() const;

                virtual ~BlDebugDrawer();

        private:
                BlProgramDebug *blProgramDebug;
                BlState *blState;

                GLuint lineBuffer;
                GLuint colorBuffer;

                std::vector<btVector3> lines;
                std::vector<btVector3> colors;
};

#endif
