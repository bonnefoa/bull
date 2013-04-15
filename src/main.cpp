#include <bl_input.h>
#include <bl_window.h>
#include <GL/gl.h>

BlInput bl_input;
BlWindow bl_window;

void render()
{
        glClearColor ( 1.0, 0.0, 0.0, 1.0 );
        glClear ( GL_COLOR_BUFFER_BIT );
        SDL_GL_SwapWindow(bl_window.window);
}

void main_loop()
{
        bl_input.poll_input();
        bl_input.handle_input();
        render();
}

int main()
{
        bl_window.launch();
        while(true) {
                main_loop();
                if(bl_input.game_state == 1) {
                        bl_window.shutdown();
                        return 0;
                }
        }
        return 1;
}
