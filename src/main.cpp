#include <bl_input.h>
#include <bl_render.h>
#include <bl_window.h>
#include <bl_simulation.h>
#include <bl_program_model.h>

BlInput *blInput;
BlWindow blWindow;
BlRender blRender;
BlSimulation blSimulation;
BlProgramModel *blProgramModel;

void main_loop()
{
        blInput->pollInput();
        blInput->handleInput();
        blRender.render();
}

void init()
{
        blWindow.launch();
        blRender.init();
        blInput = new BlInput();

        std::vector<BlShader*> shaders;
        BlShader *modelVertexShader = new BlShader("glsl/model_vertex.glsl"
                        , GL_VERTEX_SHADER);
        BlShader *modelFragmentShader = new BlShader("glsl/model_fragment.glsl"
                        , GL_FRAGMENT_SHADER);
        shaders.push_back(modelVertexShader);
        shaders.push_back(modelFragmentShader);

        blProgramModel = new BlProgramModel(shaders, blInput);
        blProgramModel->loadProgram();
        blProgramModel->init();
}

int main()
{
        init();
        while(true) {
                main_loop();
                if(blInput->gameState == 1) {
                        blWindow.shutdown();
                        return 0;
                }
                SDL_GL_SwapWindow(blWindow.window);
        }
        return 1;
}
