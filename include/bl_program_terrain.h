#ifndef BL_PROGRAM_TERRAIN_H
#define BL_PROGRAM_TERRAIN_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bl_program.h>
#include <bl_shader.h>
#include <bl_config.h>
#include <bl_terrain.h>
#include <bl_scene.h>

class BlProgramTerrain : public BlProgram
{
        public:
                BlProgramTerrain(std::vector<BlShader *> shaders,
                                BlConfig *_blConfig)
                        : BlProgram(shaders),
                        blConfig(_blConfig) {};

                void init(void);
                void displayScene(BlScene *scene, btTransform view,
                                btTransform projection);
                void moveLight(btVector3 newPosition);

        private:
                BlConfig *blConfig;

                GLint locModel;
                GLint locView;
                GLint locProjection;
                GLint locUVTexture;
                GLint locUVNormal;
                GLint heightmapSampler;
                GLint textureSampler;
                GLint normalmapSampler;

                GLint locVertices;
                GLint locNormal;
                GLint locTangent;
                GLint locBitangent;
};

BlProgramTerrain *getProgramTerrain(BlConfig *blConfig);

#endif
