#include "UniformBuffer.hpp"

UniformBuffer::UniformBuffer(const Material* data, unsigned int count) : m_Count(count)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Material) * count, data, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void UniformBuffer::bind() const 
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);

}

void UniformBuffer::unbind() const 
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}
