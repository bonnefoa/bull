#include <bl_render.h>
#include <GL/gl.h>

BlRender::BlRender()
{
}

void BlRender::render()
{
        glClearColor( 1.0, 0.0, 0.0, 1.0 );
        glClear( GL_COLOR_BUFFER_BIT );
}
