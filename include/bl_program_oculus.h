#ifndef BL_PROGRAM_OCULUS_H
#define BL_PROGRAM_OCULUS_H

#include <bl_program_model.h>
#include <bl_program_terrain.h>
#include <bl_program_skybox.h>
#include <bl_program_debug.h>
#include <bl_program_shadow.h>
#include <bl_debug.h>
#include <bl_program.h>
#include <bl_config.h>

class BlProgramOculus : public BlProgram
{
        public:
                BlProgramOculus(BlConfig *_blConfig,
                                std::vector<BlShader *> shaders,
                                BlProgramModel *_blProgramModel,
                                BlProgramTerrain *_blProgramTerrain,
                                BlProgramSkybox *_blProgramSkybox,
                                BlProgramShadow *_blProgramShadow,
                                BlDebug *_blDebug,
                                BlScene *_blScene) :
                        BlProgram(shaders),
                        blConfig(_blConfig),
                        blProgramModel(_blProgramModel),
                        blProgramTerrain(_blProgramTerrain),
                        blProgramSkybox(_blProgramSkybox),
                        blProgramShadow(_blProgramShadow),
                        blDebug(_blDebug),
                        blScene(_blScene) {} ;
                virtual ~BlProgramOculus();

                void init(void);
                void renderScene();

        private:
                void renderSceneToTexture();
                void initFramebuffer();

        private:
                BlConfig *blConfig;
                BlProgramModel *blProgramModel;
                BlProgramTerrain *blProgramTerrain;
                BlProgramSkybox *blProgramSkybox;
                BlProgramShadow *blProgramShadow;
                BlDebug *blDebug;
                BlScene *blScene;

                static const GLfloat quadVertices[12];

                GLuint oculusFramebuffer;
                GLuint vertexBuffer;
                GLint locVertices;
                GLint samplerTexture;
                GLuint sceneTexture;
                GLuint depthRenderBuffer;
};

BlProgramOculus *getProgramOculus(BlConfig *blConfig,
                BlProgramModel *blProgramModel,
                BlProgramTerrain *blProgramTerrain,
                BlProgramSkybox *blProgramSkybox,
                BlProgramShadow *blProgramShadow,
                BlDebug *blDebug,
                BlScene *blScene);

#endif /* end of include guard: BL_OCULUS_H */
