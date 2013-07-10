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
                BlProgramSkybox(std::vector<BlShader *> shaders,
                                BlConfig *_blConfig
                               )
                        : BlProgram(shaders),
                        blConfig(_blConfig) {};

                void init(void);
                void displayScene(BlScene *scene, btTransform view);
                void bindProjection();

        private:
                BlConfig *blConfig;

                GLint locVertices;
                GLint locModel;
                GLint locView;
                GLint locProjection;
                GLint cubemapSampler;
};

BlProgramSkybox *getProgramSkybox(BlConfig *blConfig);

#endif /* end of include guard: BL_PROGRAM_SKYBOX_H */
