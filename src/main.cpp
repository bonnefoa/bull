#include <bl_input.h>
#include <bl_render.h>
#include <bl_window.h>

BlInput bl_input;
BlWindow bl_window;
BlRender bl_render;

void main_loop()
{
        bl_input.poll_input();
        bl_input.handle_input();
        bl_render.render();

        SDL_GL_SwapWindow(bl_window.window);
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
