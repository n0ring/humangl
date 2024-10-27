#include "Renderer.hpp"

void clearError()
{
	while (glGetError() != GL_NO_ERROR);
}

void checkError()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
	}
}

void Renderer::draw(const VertexArray &va, const  IndexBuffer& ib, const Shader& shader)
{
	shader.bind();
	va.bind();
	// ib.bind();
	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
	// glDrawArrays(GL_TRIANGLES, 0, va.getCount());

}

void Renderer::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}
