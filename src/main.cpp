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
        blInput->handleInput();
        blRender->render();
        blProgramModel->displayModel(blModel);
}

void init_test_model() {
        std::vector<btVector3> vertices;
        std::vector<unsigned int> indices;
        vertices.push_back(btVector3(-1, -1, 0));
        vertices.push_back(btVector3(-1, 1, 0));
        vertices.push_back(btVector3(1, -1, 0));
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        blModel = new BlModel(vertices, indices);
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

        init_test_model();
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
