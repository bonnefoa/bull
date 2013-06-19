#ifndef BL_PROGRAM_TERRAIN_H
#define BL_PROGRAM_TERRAIN_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bl_program.h>
#include <bl_shader.h>
#include <bl_config.h>
#include <bl_terrain.h>
#include <bl_scene.h>
#include <bl_camera.h>

class BlProgramTerrain : public BlProgram
{
        public:
                BlProgramTerrain(std::vector<BlShader *> shaders,
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

                GLint locModel;
                GLint locView;
                GLint locProjection;
                GLint locUVTexture;
                GLint locUVNormal;
                GLint heightmapSampler;
                GLint textureSampler;
                GLint normalmapSampler;

                GLint locVertices;
                GLint locTangent;
                GLint locCotangent;
};

BlProgramTerrain *getProgramTerrain(BlConfig *blConfig, BlCamera *blCamera);

#endif
