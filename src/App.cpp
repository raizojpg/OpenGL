#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error with GLEW\n";
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float pos_image[] = {
           -50.0f, -50.0f, 0.0f, 0.0f,
            50.0f, -50.0f, 1.0f, 0.0f,
            50.0f,  50.0f, 1.0f, 1.0f,
           -50.0f,  150.0f, 0.0f, 1.0f
        };

        float positions[] = {
            -0.5f, -0.5f, // SW
             0.5f, -0.5f, // SE
             0.5f,  0.5f, // NE
            -0.5f,  0.5f, // NW
             0.0f,  0.65f,// N
             0.0f, -0.65f,// S
            -0.65f, 0.0f, // W
             0.65f, 0.0f,  // E

            -0.65f, -0.75f, // SW
             0.0f, -0.85f,  // S
             0.65f, -0.75f, // SE
             0.85f, 0.0f,   // E
             0.65f,  0.75f, // NE
             0.0f,  0.85f,  // N
            -0.65f,  0.75f, // NW
            -0.85f,  0.0f    // W
        };
        unsigned int indices_for_square[] = {
            0,1,2,
            2,3,0,
        };
        unsigned int indices_for_lines[] = {
            0,1,
            1,2,
            2,3,
            3,0,
            3,4,
            4,2,
            0,5,
            1,5,
            0,6,
            3,6,
            1,7,
            2,7
        };
        unsigned int indices_for_outline[] = {
            8,9,
            9,10,
            10,11,
            11,12,
            12,13,
            13,14,
            14,15,
            15,8
        };
        unsigned int indices_for_triangles[] = {
            11,2,7,
            11,1,7,
            15,3,6,
            15,0,6
        };


        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        //VertexBuffer vb(positions, 16 * 2 * sizeof(float));
        VertexBuffer vb(pos_image, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        vb.Unbind(); //we can unbind the buffer because the data is stored in the vertexAttrib


        IndexBuffer ib_square(indices_for_square, 2 * 3);
        //IndexBuffer ib_line(indices_for_lines, 12 * 2);
        //IndexBuffer ib_outline(indices_for_outline, 8 * 2);
        //IndexBuffer ib_triangles(indices_for_triangles, 4 * 3);

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::vec3 translationA(100, 100, 0);
        glm::vec3 translationB(300, 100, 0);


        Shader shader("res/shader/basic.shader");
        shader.Bind();

       
        //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
        float red = 0.0f;
        float increment = 0.05f;

        Texture texture("res/textures/icon.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        Renderer renderer;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls 
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");


       
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            /*
            if (red > 1.0f or red < 0.0f) {
                increment *= (-1);
            }
            red += increment;
            shader.Bind();
            shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);
            */

            shader.Bind();
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(GL_TRIANGLES, va, ib_square, shader);
            }
            
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(GL_TRIANGLES, va, ib_square, shader);
            }
            
            //renderer.Draw(GL_LINES, va, ib_line, shader);
            //renderer.Draw(GL_LINES, va, ib_outline, shader);
            //renderer.Draw(GL_TRIANGLES, va, ib_triangles, shader);


            {
                ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);  
                ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}