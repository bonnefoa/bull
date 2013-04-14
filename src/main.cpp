#include <bl_input.h>
#include <bl_sdl.h>

BlInput bl_input;
BlSdl bl_sdl;

void main_loop()
{
        bl_input.poll_input();
        bl_input.handle_input();
        if(bl_input.game_state == 0) {
                bl_sdl.shutdown();
                return;
        }
}

int main()
{
        bl_sdl.launch();
        while(true) {
                main_loop();
        }
        return 0;
}
