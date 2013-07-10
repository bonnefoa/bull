#ifndef BL_PROGRAM_SKYBOX_H
#define BL_PROGRAM_SKYBOX_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bl_program.h>
#include <bl_shader.h>
#include <bl_config.h>
#include <bl_scene.h>

class BlProgramSkybox : public BlProgram
{
        public:
                BlProgramSkybox(std::vector<BlShader *> shaders)
                        : BlProgram(shaders) {};

                void init(void);
                void displayScene(BlScene *scene, btTransform view,
                                btTransform projection);

        private:
                GLint locVertices;
                GLint locModel;
                GLint locView;
                GLint locProjection;
                GLint cubemapSampler;
};

BlProgramSkybox *getProgramSkybox();

#endif /* end of include guard: BL_PROGRAM_SKYBOX_H */
