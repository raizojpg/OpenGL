#include "TestImage.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

    TestImage::TestImage() : m_TranslationA{ 100,100,0 }, m_TranslationB{ 300,100,0 },
        m_Red{ 0.0f }, m_Green{ 0.3f }, m_Blue{ 0.8f }, m_Increment{ 0.025f }
    {  

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
		
		m_VBO = std::make_unique<VertexBuffer>(positions, 16 * 2 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->AddBuffer(*m_VBO, layout);
		
		m_IBO_square = std::make_unique<IndexBuffer>(indices_for_square, 2 * 3);
        m_IBO_lines = std::make_unique<IndexBuffer>(indices_for_lines, 12 * 2);
        m_IBO_outline = std::make_unique<IndexBuffer>(indices_for_outline, 8 * 2);
        m_IBO_triangles = std::make_unique<IndexBuffer>(indices_for_triangles, 4 * 3);

		m_Shader = std::make_unique<Shader>("res/shader/basic.shader");
		m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		m_Proj = glm::ortho(0.0f, 10.0f, 0.0f, 8.0f, -1.0f, 1.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	}

	TestImage::~TestImage(){}
	void TestImage::OnUpdate(float deltaTime){}
	
	void TestImage::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
	    
        if (bool_red) {
            IncrementColor(m_Red);
        }
        else if (bool_green) {
            IncrementColor(m_Green);
        }
        else if (bool_blue) {
            IncrementColor(m_Blue);
        }

        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", m_Red, m_Green, m_Blue, 1.0f);

        renderer.Draw(GL_TRIANGLES, *m_VAO, *m_IBO_square, *m_Shader);
        renderer.Draw(GL_LINES, *m_VAO, *m_IBO_lines, *m_Shader);
        renderer.Draw(GL_LINES, *m_VAO, *m_IBO_outline, *m_Shader);
        renderer.Draw(GL_TRIANGLES, *m_VAO, *m_IBO_triangles, *m_Shader);

	}
	
	void TestImage::OnImGuiRender()
	{
        ImGui::Checkbox("Red", &bool_red);
        ImGui::Checkbox("Green", &bool_green);
        ImGui::Checkbox("Blue", &bool_blue);
        ImGui::SliderFloat("Increment", &m_Increment, -0.1f, 0.1f);
        ImGui::SliderFloat("Red", &m_Red, 0.0f, 1.0f);
        ImGui::SliderFloat("Green", &m_Green, 0.0f, 1.0f);
        ImGui::SliderFloat("Blue", &m_Blue, 0.0f, 1.0f);
	}
    
    void TestImage::IncrementColor(float& color)
    {
        if (color > 1.0f || color < 0.0f) {
            m_Increment *= (-1);
        }
        color += m_Increment;
    }
}