#ifndef BL_PROGRAM_SKYBOX_H
#define BL_PROGRAM_SKYBOX_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bl_program.h>
#include <bl_shader.h>
#include <bl_config.h>
#include <bl_camera.h>
#include <bl_scene.h>

class BlProgramSkybox : public BlProgram
{
        public:
                BlProgramSkybox(std::vector<BlShader *> shaders,
                                BlConfig *_blConfig,
                                BlCamera *_blCamera
                               )
                        : BlProgram(shaders),
                        blConfig(_blConfig),
                        blCamera(_blCamera) {};

                void init(void);
                void displayScene(BlScene *scene);
                void bindProjection();

        private:
                BlConfig *blConfig;
                BlCamera *blCamera;

                GLint locVertices;
                GLint locModel;
                GLint locView;
                GLint locProjection;
                GLint cubemapSampler;
};

BlProgramSkybox *getProgramSkybox(BlConfig *blConfig, BlCamera *blCamera);

#endif /* end of include guard: BL_PROGRAM_SKYBOX_H */
