#pragma once

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

namespace test {
	
	class TestImage: public Test {
	public:
		TestImage();
		~TestImage();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO_square;
		std::unique_ptr<IndexBuffer> m_IBO_lines;
		std::unique_ptr<IndexBuffer> m_IBO_outline;
		std::unique_ptr<IndexBuffer> m_IBO_triangles;
		std::unique_ptr<Shader> m_Shader;

		float m_Red, m_Green, m_Blue, m_Increment;
		bool bool_red, bool_green, bool_blue;

		glm::vec3 m_TranslationA, m_TranslationB;
		glm::mat4 m_Proj, m_View;
	
		void IncrementColor(float& color);

	};

}