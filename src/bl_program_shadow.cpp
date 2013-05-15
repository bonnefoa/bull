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
        glUseProgram(programId);

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

        locDepthVP = glGetUniformLocation(programId, "depthVP");
        locDepthM = glGetUniformLocation(programId, "depthM");
        locVertices = glGetAttribLocation(programId, "vertexPosition_modelspace");

        INFO("depth vp location %i\n", locDepthVP);
        INFO("depth M location %i\n", locDepthM);
        INFO("vertex location %i\n", locVertices);
        if(locDepthVP < 0 || locVertices < 0) {
                ERROR("A location is unused");
        }

        depthProjectionMatrix = computeOrthogonal(-10, 10, -10, 10, -10, 10);
        moveLight(lightPosition);

        glUseProgram(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BlProgramShadow::moveLight(btVector3 newPosition)
{
        lightPosition = newPosition;

        glUseProgram(programId);
        btVector3 direction = -btVector3(lightPosition).normalize();
        btVector3 right = btVector3(1, 0, 0);
        btVector3 up = right.cross(direction);
        btTransform depthViewMatrix = computeView(right, up,
                        direction, lightPosition);
        sendTransform(depthProjectionMatrix * depthViewMatrix, locDepthVP);
}

void BlProgramShadow::displaySceneForRender(BlScene *blScene)
{
        glUseProgram(programId);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);

        glViewport(0, 0, 1024, 1024);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (std::vector<BlModel*>::iterator it = blScene->blModels->begin();
                        it != blScene->blModels->end(); ++it) {
                BlModel *model = (*it);
                model->bindModelMatrix(locDepthM);
                model->bindVertices(locVertices);
                model->drawElement();
        }

        glDisableVertexAttribArray(locVertices);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
