#include "bl_program_shadow.h"
#include <bl_log.h>
#include <bl_matrix.h>

BlProgramShadow *getProgramShadow(btVector3 lightPosition)
{
        std::vector<BlShader*> shaders;
        BlShader *modelVertexShader = new BlShader("glsl/shadow.vert"
                        , GL_VERTEX_SHADER);
        BlShader *modelFragmentShader = new BlShader("glsl/shadow.frag"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(modelVertexShader);
        shaders.push_back(modelFragmentShader);

        BlProgramShadow *blProgramShadow = new BlProgramShadow(shaders, lightPosition);
        blProgramShadow->loadProgram();
        blProgramShadow->init();
        return blProgramShadow;
}

void BlProgramShadow::init(void)
{
        glUseProgram(programId);

        glGenFramebuffers(1, &shadowFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);

        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024,
                        1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                        GL_TEXTURE_2D, depthTexture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE) {
                ERROR("Framebuffer not complete\n");
        }

        locDepthVP = glGetUniformLocation(programId, "depthVP");
        locDepthM = glGetUniformLocation(programId, "depthM");
        locVertices = glGetAttribLocation(programId, "vertexPosition_modelspace");

        INFO("depth vp location %i\n", locDepthVP);
        INFO("depth M location %i\n", locDepthM);
        INFO("vertex location %i\n", locVertices);
        if(locDepthVP < 0 || locVertices < 0 || locDepthM < 0) {
                ERROR("A location is unused");
        }

        moveLight(lightPosition);

        glUseProgram(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

BlProgramShadow::~BlProgramShadow()
{
        glDeleteFramebuffers(1, &shadowFramebuffer);
        glDeleteTextures(1, &depthTexture);
}

void BlProgramShadow::moveLight(btVector3 newPosition)
{
        lightPosition = newPosition;
        glUseProgram(programId);
        btTransform shadowVP = computeVPShadowMatrix(newPosition);
        sendTransform(shadowVP, locDepthVP);
}

void BlProgramShadow::displaySceneForRender(BlScene *blScene)
{
        glUseProgram(programId);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);

        glClear(GL_DEPTH_BUFFER_BIT);

        for (std::vector<BlModel*>::iterator it = blScene->blModels->begin();
                        it != blScene->blModels->end(); ++it) {
                BlModel *model = (*it);
                model->drawElement(locDepthM, locVertices,
                                -1, -1);
        }

        glDisableVertexAttribArray(locVertices);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(0);
}
