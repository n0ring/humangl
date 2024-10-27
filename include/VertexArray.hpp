#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"


class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout, unsigned int count);
	void bind() const;
	void unbind() const;
	inline unsigned int getCount() const {return m_count;}

private:
	unsigned int m_renderID;
	unsigned int m_count;
};