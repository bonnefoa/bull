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

class  BlProgramModel : public BlProgram
{
        public:
                BlProgramModel(std::vector<BlShader *> shaders
                                , BlInput *_blInput
                                , BlConfig *_blConfig) :
                        BlProgram(shaders), blInput(_blInput), blConfig(_blConfig) {};
                void init(void);
                void bindModelMatrix(BlModel *blModel);
                void displayModel(BlModel *model);
                void displayScene(BlScene *blScene, GLuint depthTexture);
                void moveLight(btVector3 position);
                void bindProjection();

        private:
                BlInput *blInput;
                BlConfig *blConfig;
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

BlProgramModel *getProgramModel(BlInput *blInput, BlConfig *blConfig);

#endif
