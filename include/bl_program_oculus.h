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

typedef enum {
        FullEye,
        LeftEye,
        RightEye
} oculusEye_t;

typedef struct viewport {
        int width;
        int height;
        btTransform projection;
        oculusEye_t eye;
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
                void initFramebuffer(GLuint *framebuffer, GLuint *sceneTexture,
                                GLuint *depthRender, viewport_t viewport);
                void initViewports();
                void renderEyeScene(viewport_t viewport);
                void initBuffers(GLuint *vertexBuffer, const GLfloat *quadVertices,
                                GLuint *uvBuffer, const GLfloat *quadUV);

        private:
                BlConfig *blConfig;
                BlCamera *blCamera;
                BlProgramModel *blProgramModel;
                BlProgramTerrain *blProgramTerrain;
                BlProgramSkybox *blProgramSkybox;
                BlProgramShadow *blProgramShadow;
                BlDebug *blDebug;
                BlScene *blScene;

                static const GLfloat quadVerticesLeft[12];
                static const GLfloat quadVerticesRight[12];
                static const GLfloat quadUVLeft[8];
                static const GLfloat quadUVRight[8];

                GLuint oculusFramebufferRight;
                GLuint oculusFramebufferLeft;
                GLuint sceneTextureRight;
                GLuint sceneTextureLeft;
                GLuint depthRenderBufferRight;
                GLuint depthRenderBufferLeft;

                GLuint uvBufferLeft;
                GLuint uvBufferRight;

                GLuint vertexBufferLeft;
                GLuint vertexBufferRight;

                GLint locVertices;
                GLint locSampler;
                GLint locUV;

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
