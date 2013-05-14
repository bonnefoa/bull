#include "bl_program_shadow.h"
#include <bl_log.h>
#include <bl_matrix.h>

BlProgramShadow *getProgramShadow()
{
        std::vector<BlShader*> shaders;
        BlShader *modelVertexShader = new BlShader("glsl/shadow_vertex.glsl"
                        , GL_VERTEX_SHADER);
        BlShader *modelFragmentShader = new BlShader("glsl/shadow_fragment.glsl"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(modelVertexShader);
        shaders.push_back(modelFragmentShader);

        BlProgramShadow *blProgramShadow = new BlProgramShadow(shaders, btVector3(0, 0, 0));
        blProgramShadow->loadProgram();
        blProgramShadow->init();
        return blProgramShadow;
}

void BlProgramShadow::init(void)
{
        glGenFramebuffers(1, &shadowFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);

        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024,
                        1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                        GL_TEXTURE_2D, depthTexture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE) {
                ERROR("Framebuffer not complete\n");
        }

        glUseProgram(programId);

        locDepthMVP = glGetUniformLocation(programId, "depthMVP");
        locVertices = glGetAttribLocation(programId, "vertexPosition_modelspace");

        INFO("depth mvp location %i\n", locDepthMVP);
        INFO("vertex location %i\n", locVertices);
        if(locDepthMVP < 0 || locVertices < 0) {
                ERROR("A location is unused");
        }

        depthProjectionMatrix = computeOrthogonal(-10, 10, -10, 10, -10, 20);
        moveLight(lightPosition);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BlProgramShadow::moveLight(btVector3 position)
{
        lightPosition = position;
        btTransform depthViewMatrix = computeView(btVector3(1, 0, 0), btVector3(0, 1, 0),
                        -lightPosition, lightPosition);
        sendTransform(depthProjectionMatrix * depthViewMatrix, locDepthMVP);
}

void BlProgramShadow::displaySceneForRender(BlScene *blScene)
{
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);

        glClear(GL_DEPTH_BUFFER_BIT);
        glUseProgram(programId);

        (void) blScene;
        for (std::vector<BlModel*>::iterator it = blScene->blModels->begin();
                        it != blScene->blModels->end(); ++it) {
                BlModel *model = (*it);
                model->bindVertices(locVertices);
                model->drawElement();
        }

        glDisableVertexAttribArray(locVertices);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


