#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



namespace test {
	TestTexture2D::TestTexture2D() : m_TranslationA{100,100,0}, m_TranslationB{300,100,0}
 {
	
		float pos_image[] = {
		  -50.0f, -50.0f, 0.0f, 0.0f,
		   50.0f, -50.0f, 1.0f, 0.0f,
		   50.0f,  50.0f, 1.0f, 1.0f,
		  -50.0f,  150.0f, 0.0f, 1.0f
		};
		unsigned int indices_for_square[] = {
		   0,1,2,
		   2,3,0,
		};
		
		m_VBO = std::make_unique<VertexBuffer>(pos_image, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->AddBuffer(*m_VBO, layout);
		
		m_IBO = std::make_unique<IndexBuffer>(indices_for_square, 2 * 3);

		m_Shader = std::make_unique<Shader>("res/shader/texture.shader");
		m_Shader->Bind();

		m_Texture =  std::make_unique<Texture>("res/textures/icon.png");
		m_Texture->Bind();
		m_Shader->SetUniform1i("u_Texture", 0);
			
		m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	}

	TestTexture2D::~TestTexture2D(){}
	void TestTexture2D::OnUpdate(float deltaTime){}
	
	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Shader->Bind();
		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(GL_TRIANGLES, *m_VAO, *m_IBO, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(GL_TRIANGLES, *m_VAO, *m_IBO, *m_Shader);
		}

	}
	
	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("m_Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("m_Translation B", &m_TranslationB.x, 0.0f, 960.0f);
		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	}
}