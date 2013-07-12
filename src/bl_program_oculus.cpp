#include "bl_program_oculus.h"
#include <bl_log.h>
#include <bl_matrix.h>
#include <bl_gl_util.h>

const GLfloat BlProgramOculus::quadVerticesLeft[12] = {
        -1, -1, 0,
         0, -1, 0,
         0,  1, 0,
        -1,  1, 0
};

const GLfloat BlProgramOculus::quadUVLeft[8] = {
        0,  0,
        1,  0,
        1,  1,
        0,  1
};

const GLfloat BlProgramOculus::quadVerticesRight[12] = {
         0, -1, 0,
         1, -1, 0,
         1,  1, 0,
         0,  1, 0
};

const GLfloat BlProgramOculus::quadUVRight[8] = {
        0,  0,
        1,  0,
        1,  1,
        0,  1
};

BlProgramOculus *getProgramOculus(BlConfig *blConfig,
                BlCamera *blCamera,
                BlProgramModel *blProgramModel,
                BlProgramTerrain *blProgramTerrain,
                BlProgramSkybox *blProgramSkybox,
                BlProgramShadow *blProgramShadow,
                BlDebug *blDebug,
                BlScene *blScene)
{
        std::vector<BlShader*> shaders;
        BlShader *modelVertexShader = new BlShader("glsl/oculus_post_process.vert"
                        , GL_VERTEX_SHADER);
        BlShader *modelFragmentShader = new BlShader("glsl/oculus_post_process.frag"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(modelVertexShader);
        shaders.push_back(modelFragmentShader);

        BlProgramOculus *blProgramOculus = new BlProgramOculus(blConfig,
                        blCamera, shaders, blProgramModel,
                        blProgramTerrain, blProgramSkybox, blProgramShadow,
                        blDebug, blScene);
        blProgramOculus->loadProgram();
        blProgramOculus->init();
        return blProgramOculus;
}

void BlProgramOculus::initViewports()
{
        oculusConf_t conf = blConfig->oculusConf;
        int eyeWidth = conf.widthResolution / 2;
        float viewCenter         = conf.widthResolution * 0.25f;
        float eyeProjectionShift = viewCenter - conf.lensSeparationDistance *0.5f;
        btVector3 projectionCenterOffset = btVector3(
                        4.0f * eyeProjectionShift / conf.heightResolution, 0, 0);
        btTransform leftProjection = translateBtTransform(blConfig->projection,
                        projectionCenterOffset);
        btTransform rightProjection = translateBtTransform(blConfig->projection,
                        -1 *  projectionCenterOffset);

        viewportLeft.projection  = leftProjection;
        viewportLeft.width       = eyeWidth;
        viewportLeft.height      = conf.heightResolution;
        viewportLeft.eye         = LeftEye;

        viewportRight.projection = rightProjection;
        viewportRight.width      = eyeWidth;
        viewportRight.height     = conf.heightResolution;
        viewportRight.eye        = RightEye;

        viewportFull.projection  = blConfig->projection;
        viewportFull.width       = blConfig->width;
        viewportFull.height      = blConfig->height;
        viewportFull.eye         = FullEye;
}

void BlProgramOculus::initFramebuffer(GLuint *framebuffer,
                GLuint *sceneTexture, GLuint *depthRender,
                viewport_t viewport)
{
        glGenTextures(1, sceneTexture);
        glBindTexture(GL_TEXTURE_2D, *sceneTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewport.width,
                        viewport.width, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenRenderbuffers(1, depthRender);
        glBindRenderbuffer(GL_RENDERBUFFER, *depthRender);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                        viewport.width, viewport.height);

        glGenFramebuffers(1, framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                        GL_TEXTURE_2D, *sceneTexture, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                        GL_RENDERBUFFER, *depthRender);
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE) {
                ERROR("Framebuffer not complete\n");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void BlProgramOculus::initBuffers(GLuint *vertexBuffer, const GLfloat *quadVertices,
                GLuint *uvBuffer, const GLfloat *quadUV)
{
        glGenBuffers(1, vertexBuffer);
        loadArrayInBuffer(*vertexBuffer, quadVertices, sizeof(GLfloat) * 12);
        glGenBuffers(1, uvBuffer);
        loadArrayInBuffer(*uvBuffer, quadUV, sizeof(GL_FLOAT) * 8);
}

void BlProgramOculus::init()
{
        glUseProgram(programId);

        initViewports();
        glActiveTexture(GL_TEXTURE6);
        initFramebuffer(&oculusFramebufferRight, &sceneTextureRight,
                        &depthRenderBufferRight, viewportRight);
        initFramebuffer(&oculusFramebufferLeft, &sceneTextureLeft,
                        &depthRenderBufferLeft, viewportLeft);

        initBuffers(&vertexBufferRight, &quadVerticesRight[0],
                        &uvBufferRight, &quadUVRight[0]);
        initBuffers(&vertexBufferLeft, &quadVerticesLeft[0],
                        &uvBufferLeft, &quadUVLeft[0]);

        locUV = glGetAttribLocation(programId, "vertexUV");
        locVertices = glGetAttribLocation(programId, "vertexPosition");
        locSampler = glGetUniformLocation(programId, "samplerTexture");
        glUniform1i(locSampler, 6);

        INFO("UV location %i\n", locUV);
        INFO("vertex location %i\n", locVertices);
        INFO("Sampler %i\n", locSampler);
        if(locUV < 0 || locVertices < 0 || locSampler < 0) {
                ERROR("A location is unused");
        }

        glUseProgram(0);
}

void BlProgramOculus::renderSceneToTexture(viewport_t viewport, btTransform view)
{
        glViewport(0, 0, viewport.width, viewport.height);
        glClearColor( 0.0, 0.0, 0.2, 1.0 );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        blProgramSkybox->displayScene(blScene, view, viewport.projection);
        blProgramTerrain->displayScene(blScene, view, viewport.projection);
        blProgramModel->displayScene(blScene, blProgramShadow->depthTexture,
                        view, viewport.projection);
        blDebug->renderDebug(view, viewport.projection);
}

void BlProgramOculus::renderEyeScene(viewport_t viewport)
{
        btVector3 centerOffset = btVector3(
                        blConfig->oculusConf.interpupillaryDistance * 0.5f, 0, 0);
        btTransform view = blCamera->view;

        switch(viewport.eye) {
                case RightEye:
                        glBindFramebuffer(GL_FRAMEBUFFER, oculusFramebufferRight);
                        view = translateBtTransform(blCamera->view, centerOffset);
                        break;
                case LeftEye:
                        glBindFramebuffer(GL_FRAMEBUFFER, oculusFramebufferLeft);
                        view = translateBtTransform(blCamera->view, -1 * centerOffset);
                        break;
                case FullEye:
                        return;
        };
        renderSceneToTexture(viewport, view);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BlProgramOculus::renderScene()
{
        renderEyeScene(viewportRight);
        renderEyeScene(viewportLeft);

        glUseProgram(programId);
        glViewport(0, 0, viewportFull.width, viewportFull.height);
        glClearColor( 0.2, 0.2, 0.2, 1.0 );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnableVertexAttribArray(locVertices);
        glEnableVertexAttribArray(locUV);
        glActiveTexture(GL_TEXTURE6);

        glBindTexture(GL_TEXTURE_2D, sceneTextureLeft);
        bindBufferToLocation(locUV, uvBufferLeft, 2);
        bindBufferToLocation(locVertices, vertexBufferLeft, 3);
        glDrawArrays(GL_QUADS, 0, 4);

        glBindTexture(GL_TEXTURE_2D, sceneTextureRight);
        bindBufferToLocation(locUV, uvBufferRight, 2);
        bindBufferToLocation(locVertices, vertexBufferRight, 3);
        glDrawArrays(GL_QUADS, 0, 4);

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisableVertexAttribArray(locVertices);
        glDisableVertexAttribArray(locUV);
        glUseProgram(0);
}

BlProgramOculus::~BlProgramOculus()
{
        glDeleteFramebuffers(1, &oculusFramebufferRight);
        glDeleteFramebuffers(1, &oculusFramebufferLeft);
        glDeleteTextures(1, &sceneTextureLeft);
        glDeleteTextures(1, &sceneTextureRight);
        glDeleteRenderbuffers(1, &depthRenderBufferLeft);
        glDeleteRenderbuffers(1, &depthRenderBufferRight);
        glDeleteBuffers(1, &vertexBufferLeft);
        glDeleteBuffers(1, &vertexBufferRight);
        glDeleteBuffers(1, &uvBufferLeft);
        glDeleteBuffers(1, &uvBufferRight);
}
