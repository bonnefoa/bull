#include "bl_program_oculus.h"
#include <bl_log.h>
#include <bl_matrix.h>

const GLfloat BlProgramOculus::quadVertices[12] = {
        -1, -1, 0,
         1, -1, 0,
         1,  1, 0,
        -1,  1, 0
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

void BlProgramOculus::initFramebuffer()
{
        glGenTextures(1, &sceneTexture);
        glBindTexture(GL_TEXTURE_2D, sceneTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, blConfig->width,
                        blConfig->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenRenderbuffers(1, &depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                        blConfig->width, blConfig->height);

        glGenFramebuffers(1, &oculusFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, oculusFramebuffer);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                        GL_TEXTURE_2D, sceneTexture, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                        GL_RENDERBUFFER, depthRenderBuffer);
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE) {
                ERROR("Framebuffer not complete\n");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BlProgramOculus::initViewports()
{
        int eyeWidth = blConfig->width / 2;

        float lensSeparationDistance = 0.0635f;
        float viewCenter         = blConfig->width * 0.25f;
        float eyeProjectionShift = viewCenter - lensSeparationDistance*0.5f;
        btVector3 projectionCenterOffset = btVector3(
                        4.0f * eyeProjectionShift / blConfig->height, 0, 0);
        btTransform leftProjection = translateBtTransform(blConfig->projection,
                       projectionCenterOffset);
        btTransform rightProjection = translateBtTransform(blConfig->projection,
                        -1 *  projectionCenterOffset);

        viewportLeft.projection  = leftProjection;
        viewportLeft.width       = eyeWidth;
        viewportLeft.height      = blConfig->height;
        viewportLeft.x           = 0;
        viewportLeft.y           = 0;

        viewportRight.projection = rightProjection;
        viewportRight.width      = eyeWidth;
        viewportRight.height     = blConfig->height;
        viewportRight.x          = eyeWidth;
        viewportRight.y          = 0;

        viewportFull.projection  = blConfig->projection;
        viewportFull.width       = blConfig->width;
        viewportFull.height      = blConfig->height;
        viewportFull.x           = 0;
        viewportFull.y           = 0;
}

void BlProgramOculus::init()
{
        glUseProgram(programId);

        initFramebuffer();
        initViewports();

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices),
                        quadVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        locVertices = glGetAttribLocation(programId, "vertexPosition");
        locSampler = glGetUniformLocation(programId, "samplerTexture");
        locTextureModel = glGetUniformLocation(programId, "textureModel");
        glUniform1i(locSampler, 6);
}

void BlProgramOculus::renderSceneToTexture(viewport_t viewport, btTransform view)
{
        glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
        blProgramSkybox->displayScene(blScene, view, viewport.projection);
        blProgramTerrain->displayScene(blScene, view, viewport.projection);
        blProgramModel->displayScene(blScene, blProgramShadow->depthTexture,
                        view, viewport.projection);
        blDebug->renderDebug(view, viewport.projection);
}

void BlProgramOculus::renderScene()
{
        glBindFramebuffer(GL_FRAMEBUFFER, oculusFramebuffer);
        glClearColor( 0.0, 0.0, 0.2, 1.0 );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        btVector3 centerOffset = btVector3(blConfig->interpupillaryDistance * 0.5f, 0, 0);

        btTransform leftView = translateBtTransform(blCamera->view, -1 * centerOffset);
        btTransform rightView = translateBtTransform(blCamera->view, centerOffset);

        renderSceneToTexture(viewportLeft, leftView);
        renderSceneToTexture(viewportRight, rightView);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glUseProgram(programId);
        glViewport(viewportFull.x, viewportFull.y,
                        viewportFull.width, viewportFull.height);

        glClearColor( 0.0, 0.0, 0.2, 1.0 );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, sceneTexture);

        glEnableVertexAttribArray(locVertices);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(locVertices, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_QUADS, 0, 4);

        glDisableVertexAttribArray(locVertices);
        glUseProgram(0);
}

BlProgramOculus::~BlProgramOculus()
{
        glDeleteFramebuffers(1, &oculusFramebuffer);
        glDeleteTextures(1, &sceneTexture);
        glDeleteBuffers(1, &vertexBuffer);
}
