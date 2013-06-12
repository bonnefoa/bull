#ifndef BL_PROGRAM_SHADOW_H
#define BL_PROGRAM_SHADOW_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bl_program.h>
#include <bullet.h>
#include <bl_scene.h>

class BlProgramShadow : public BlProgram {
        public:
                BlProgramShadow(std::vector<BlShader *> shaders, btVector3 _lightPosition)
                        : BlProgram (shaders), lightPosition(_lightPosition) {};
                void init(void);
                void displaySceneForRender(BlScene *scene);
                void moveLight(btVector3 position);
                void displayTexture(GLuint textureId);

                btVector3 lightPosition;
                GLuint depthTexture;
                virtual ~BlProgramShadow();

        private:
                GLuint shadowFramebuffer;

                GLint locDepthVP;
                GLint locDepthM;
                GLint locVertices;

                btTransform depthProjectionMatrix;
};

BlProgramShadow *getProgramShadow(btVector3 lightPosition);

#endif
