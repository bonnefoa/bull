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

typedef struct viewport {
        int width;
        int height;
        int x;
        int y;
        btTransform projection;
} viewport_t;

class BlProgramOculus : public BlProgram
{
        public:
                BlProgramOculus(BlConfig *_blConfig,
                                BlCamera *_blCamera,
                                std::vector<BlShader *> shaders,
                                BlProgramModel *_blProgramModel,
                                BlProgramTerrain *_blProgramTerrain,
                                BlProgramSkybox *_blProgramSkybox,
                                BlProgramShadow *_blProgramShadow,
                                BlDebug *_blDebug,
                                BlScene *_blScene) :
                        BlProgram(shaders),
                        blConfig(_blConfig),
                        blCamera(_blCamera),
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
                void renderSceneToTexture(viewport_t viewport, btTransform view);
                void initFramebuffer();
                void initViewports();

        private:
                BlConfig *blConfig;
                BlCamera *blCamera;
                BlProgramModel *blProgramModel;
                BlProgramTerrain *blProgramTerrain;
                BlProgramSkybox *blProgramSkybox;
                BlProgramShadow *blProgramShadow;
                BlDebug *blDebug;
                BlScene *blScene;

                static const GLfloat quadVertices[12];

                GLuint oculusFramebuffer;
                GLuint vertexBuffer;
                GLint locTextureModel;
                GLint locVertices;
                GLint locSampler;
                GLuint sceneTexture;
                GLuint depthRenderBuffer;

                viewport_t viewportLeft;
                viewport_t viewportRight;
                viewport_t viewportFull;
};

BlProgramOculus *getProgramOculus(BlConfig *blConfig,
                BlCamera *blCamera,
                BlProgramModel *blProgramModel,
                BlProgramTerrain *blProgramTerrain,
                BlProgramSkybox *blProgramSkybox,
                BlProgramShadow *blProgramShadow,
                BlDebug *blDebug,
                BlScene *blScene);

#endif /* end of include guard: BL_OCULUS_H */
