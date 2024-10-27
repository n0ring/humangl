#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"

void clearError();
void checkError();

class Renderer
{
public:
	void draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader);
	void clear() const;
private:
};
