#include "VertexArray.hpp"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_renderID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_renderID);
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout, unsigned int count)
{
	bind();
	vb.bind();
	m_count = count;
	unsigned long offset = 0;
	const auto& elements = layout.getElements();
	for (size_t i = 0; i < elements.size(); ++i)
	{
		 const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.getStride(), (const void*) offset); // 0 is the index of the vertex attribute
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

void VertexArray::bind() const
{
	glBindVertexArray(m_renderID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}
