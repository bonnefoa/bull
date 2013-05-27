#ifndef BL_PROGRAM_TERRAIN_H
#define BL_PROGRAM_TERRAIN_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bl_program.h>
#include <bl_shader.h>
#include <bl_config.h>
#include <bl_state.h>
#include <bl_terrain.h>
#include <bl_scene.h>

class BlProgramTerrain : public BlProgram
{
        public:
                BlProgramTerrain(std::vector<BlShader *> shaders,
                                BlConfig *_blConfig,
                                BlState *_blState
                                )
                        : BlProgram(shaders),
                        blConfig(_blConfig),
                        blState(_blState) {};

                void init(void);
                void displayScene(BlScene *scene);
                void displayTerrain(BlTerrain *terrain);

                void bindProjection();

        private:
                BlConfig *blConfig;
                BlState *blState;

                GLint locModel;
                GLint locView;
                GLint locProjection;
                GLint locGridLenght;
                GLint locGridWidth;
                GLint locUVTexture;
                GLint heightmapSampler;
                GLint textureSampler;

                GLint locVertices;
};

BlProgramTerrain *getProgramTerrain(BlConfig *blConfig, BlState *blState);

#endif
