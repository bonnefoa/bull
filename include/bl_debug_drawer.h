#ifndef BL_DEBUG_DRAWER_H
#define BL_DEBUG_DRAWER_H

#include <bullet.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <bl_program_debug.h>
#include <bl_state.h>
#include <bl_text.h>

class BlDebugDrawer : public btIDebugDraw {
        public:
                BlDebugDrawer(BlProgramDebug *_blProgramDebug,
                                BlState *_blState,
                                BlText *_blText)
                        : blProgramDebug(_blProgramDebug),
                        blState(_blState),
                        blText(_blText) {} ;
                void init();
                virtual void drawLine(const btVector3& from,
                                const btVector3& to,const btVector3& color);
                virtual void drawContactPoint(const btVector3& PointOnB,
                                const btVector3& normalOnB,btScalar distance,
                                int lifeTime,const btVector3& color);
                virtual void reportErrorWarning(const char* warningString);
                virtual void draw3dText(const btVector3& location, const char* textString);
                virtual void setDebugMode(int debugMode);
                virtual int getDebugMode() const;
                void initDebugRender(btTransform view, btTransform projection);
                void finalizeDraw();
                void drawAxis(const btTransform& center,
                                const btVector3& direction,
                                const btVector3& color);
                void drawXYZAxis(const btTransform& center);

                virtual ~BlDebugDrawer();
                GLuint textureBuffer;

        private:
                BlProgramDebug *blProgramDebug;
                BlState *blState;
                BlText *blText;

                GLuint lineBuffer;
                GLuint colorBuffer;

                GLuint vertexBuffer;
                GLuint uvBuffer;

                std::vector<btVector3> lines;
                std::vector<btVector3> colors;

                std::vector<btVector3> textVertices;
                std::vector<float> textUvs;
};

#endif
