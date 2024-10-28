#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

class OpenGlSetter
{
public:
	OpenGlSetter();
	~OpenGlSetter();

	void setKeyboardCallback(GLFWkeyfun callback);

	bool isAppRunning();
	void swapBuffersPollEvents();
	void clearScreen();
	inline bool getState() const { return m_initStatus; }
	float getDeltaTime();

private:
	GLFWwindow *m_window;
	bool m_initStatus = true;
	float m_lastFrame = 0.0f;
};