#pragma once 

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "ParsedObject.hpp"


class UniformBuffer 
{
public: 
	UniformBuffer(const Material* data, unsigned int count);
	~UniformBuffer();

	void bind() const;
	void unbind() const;
	inline unsigned int getCount() const { return m_Count; }
	inline unsigned int getRendererId() const {return m_RendererID; }

private: 
	unsigned int m_RendererID;
	unsigned int m_Count;
};