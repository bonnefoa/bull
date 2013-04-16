#include <bl_render.h>

BlRender::BlRender()
{
}

void BlRender::init()
{
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDepthFunc(GL_LESS);

}

void BlRender::render()
{
        glClearColor( 1.0, 0.0, 0.0, 1.0 );
        glClear( GL_COLOR_BUFFER_BIT );
}
