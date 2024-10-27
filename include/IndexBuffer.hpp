#pragma once 

#include "glad/glad.h"
#include <GLFW/glfw3.h>


class IndexBuffer 
{
public: 
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return m_Count; }

private: 
	unsigned int m_RendererID;
	unsigned int m_Count;
};