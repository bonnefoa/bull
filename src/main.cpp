#include <bl_input.h>
#include <bl_window.h>
#include <bl_simulation.h>
#include <bl_program_model.h>

BlInput *blInput;
BlWindow *blWindow;
BlSimulation *blSimulation;
BlProgramModel *blProgramModel;
BlModel *blModel;

void init_test_model() {
        std::vector<btVector3> vertices;
        std::vector<unsigned int> indices;
        vertices.push_back(btVector3(-8.0f, -8.0f, 0.0f));
        vertices.push_back(btVector3(8.0f, -8.0f, 0.0f));
        vertices.push_back(btVector3(0.0f, 8.0f, 0.0f));
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        blModel = new BlModel(vertices, indices);
        blModel->init();
}

void init()
{
        blWindow = new BlWindow();
        blWindow->launch();
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

void main_loop()
{
        glClearColor( 0.0, 0.0, 0.2, 1.0 );
        glClear( GL_COLOR_BUFFER_BIT );
        blInput->handleInput();
        blProgramModel->displayModel(blModel);
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
