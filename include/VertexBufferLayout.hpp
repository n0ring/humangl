#pragma once

#include <vector>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
		#include <iostream>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		case GL_INT:
			return 4;
		}
		// assert(false);
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout() : m_stride(0) {}
	~VertexBufferLayout() {}

	template <typename T>
	void push(unsigned int count)
	{
		(void)count;
		// static_assert(false);
	}

	template <>
	void push<float>(unsigned int count)
	{
		m_elements.push_back({GL_FLOAT, count, GL_FALSE});
		m_stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
	}
	template <>
	void push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({GL_UNSIGNED_INT, static_cast<unsigned int>(count), GL_FALSE});
		m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template <>
	void push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
		m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
	}
	template <>
	void push<int>(unsigned int count)
	{
		m_elements.push_back({GL_INT, count, GL_TRUE});
		m_stride += VertexBufferElement::getSizeOfType(GL_INT) * count;
	}
	inline const std::vector<VertexBufferElement> &getElements() const { return m_elements; }
	inline unsigned int getStride() const { return m_stride; }

private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
};