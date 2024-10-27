#include "test/TestTexture.hpp"

namespace test
{
	TestTexture::TestTexture()
		: m_translationA(200, 200, 0),
		  m_translationB(400, 200, 0),
		  m_proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		  m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		std::vector<float> positions = {
			-50.0f, -50.00f, 0.0f, 0.0f, // 0
			50.0f, -50.0f, 1.0f, 0.0f, // 1
			50.0f, 50.0f, 1.0f, 1.0f, // 2
			-50.0f, 50.0f, 0.0f, 1.0f, // 3
		};

		std::vector<unsigned int> indeces = {
			0, 1, 2,
			0, 2, 3};

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(positions.data(), positions.size() * sizeof(float));
		VertexBufferLayout layouts;
		layouts.push<float>(2);
		layouts.push<float>(2);
		m_VAO->addBuffer(*m_VBO, layouts);
		m_IBO = std::make_unique<IndexBuffer>(indeces.data(), indeces.size());

		m_shader = std::make_unique<Shader>("res/shaders/Basic.shader");

		m_shader->bind();

		m_texture = std::make_unique<Texture>("res/texture/texture.png");
		m_texture->bind();
		m_shader->setUniform1i("u_Texture", 0);
	}
	TestTexture::~TestTexture()
	{
	}
	void TestTexture::onUpdate(float deltaTime)
	{
		(void)deltaTime;
	}
	void TestTexture::onRender()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Renderer renderer;
		m_texture->bind();
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
			glm::mat4 mvp = m_proj * m_view * model;
			m_shader->bind();
			m_shader->setUniformMat4f("u_MVP", mvp);
			clearError();
			renderer.draw(*m_VAO, *m_IBO, *m_shader);
			checkError();
		}
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
			glm::mat4 mvp = m_proj * m_view * model;
			m_shader->bind();
			m_shader->setUniformMat4f("u_MVP", mvp);
			renderer.draw(*m_VAO, *m_IBO, *m_shader);
		}
	}

	void TestTexture::onImGuiRender()
	{
		ImGui::SliderFloat3("translationA", &m_translationA.x, 0.0f, 960.0f); // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat3("translationB", &m_translationB.x, 0.0f, 960.0f); // Edit 1 float using a slider from 0.0f to 1.0f
	}
}