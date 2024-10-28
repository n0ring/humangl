#include "OpenGlSetter.hpp"

OpenGlSetter::OpenGlSetter()
{
	if (!glfwInit())
	{
		m_initStatus = false;
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Necessary on Mac
	/* Create a windowed mode window and its OpenGL context */
	m_window = glfwCreateWindow(960, 540, "humangl_42", NULL, NULL);
	if (!m_window)
	{
		m_initStatus = false;
		return;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		m_initStatus = false;
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glClearColor(.5f, .5f, .5f, 1.0f);
}

OpenGlSetter::~OpenGlSetter()
{
	if (m_window)
		glfwDestroyWindow(m_window);
	glfwTerminate();
}

void OpenGlSetter::setKeyboardCallback(GLFWkeyfun callback)
{
	glfwSetKeyCallback(m_window, callback);
}

bool OpenGlSetter::isAppRunning()
{
	return !glfwWindowShouldClose(m_window);
}

void OpenGlSetter::swapBuffersPollEvents()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void OpenGlSetter::clearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

float OpenGlSetter::getDeltaTime()
{
	float currentFrame = glfwGetTime();
	float deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;
	return deltaTime;
}