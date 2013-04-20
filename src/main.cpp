#include <bl_input.h>
#include <bl_render.h>
#include <bl_window.h>
#include <bl_simulation.h>
#include <bl_program_model.h>

BlInput *blInput;
BlWindow *blWindow;
BlRender *blRender;
BlSimulation *blSimulation;
BlProgramModel *blProgramModel;
BlModel *blModel;

void main_loop()
{
        blInput->pollInput();
        blInput->handleInput();
        blRender->render();
        blProgramModel->displayModel(blModel);
}

void init()
{
        blWindow = new BlWindow();
        blWindow->launch();
        blRender = new BlRender();
        blRender->init();
        blInput = new BlInput();
        blSimulation = new BlSimulation();

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

        blModel = new BlModel("");
        blModel->loadAsset();

        blProgramModel->loadModelInBuffer(blModel);
}

int main()
{
        init();
        while(true) {
                main_loop();
                if(blInput->gameState == 1) {
                        blWindow->shutdown();
                        return 0;
                }
                SDL_GL_SwapWindow(blWindow->window);
        }
        return 1;
}
