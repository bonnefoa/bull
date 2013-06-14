#ifndef BL_PROGRAM_MODEL_H
#define BL_PROGRAM_MODEL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bl_program.h>
#include <bl_shader.h>
#include <bl_input.h>
#include <vector>
#include <bl_scene.h>
#include <bl_config.h>
#include <bl_camera.h>

class  BlProgramModel : public BlProgram
{
        public:
                BlProgramModel(std::vector<BlShader *> shaders
                                , BlInput *_blInput
                                , BlConfig *_blConfig
                                , BlState *_blState
                                , BlCamera *_blCamera
                                ) :
                        BlProgram(shaders),
                        blInput(_blInput),
                        blConfig(_blConfig),
                        blState(_blState),
                        blCamera(_blCamera) {};
                void init(void);
                void bindModelMatrix(BlModel *blModel);
                void displayModel(BlModel *model);
                void displayCharacter(BlCharacter *blCharacter);
                void displayScene(BlScene *blScene, GLuint depthTexture);
                void moveLight(btVector3 position);
                void bindProjection();

        private:
                BlInput *blInput;
                BlConfig *blConfig;
                BlState *blState;
                BlCamera *blCamera;

                GLint locModel;
                GLint locView;
                GLint locProjection;
                GLint locShadowVP;

                GLint locVertices;
                GLint locNormals;
                GLint locUVs;

                GLint samplerTexture;
                GLint samplerShadow;
};

BlProgramModel *getProgramModel(BlInput *blInput,
                BlConfig *blConfig,
                BlState *blState,
                BlCamera *blCamera);

#endif
