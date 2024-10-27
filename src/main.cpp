#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
// #include "ParsedObject.hpp"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "KeyboardManager.hpp"

#include "Model.hpp"
#include "Animator.hpp"
#include <unistd.h>

#include "AnimationConfigParser.hpp"
// #include "glm_inc.hpp"


std::string animationNames[] = {"idle", "jump", "walk", "kick"};
// ModelState g_modelState;
KeyboardManager keyboardManager;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	keyboardManager.setKeyState(key, action);
} 

int main(int argc, char** argv)
{
	GLFWwindow *window;
	std::vector<Model> objects;
	unsigned int objId = 0;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Necessary on Mac

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "humangl_42", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	std::unordered_map<std::string, std::string> configMap;

	// std::string configFileName = "animations.conf";
	std::string configFileName = "humangl_animations.conf"; 
	if (!AnimationConfigParser::parseConfig(configFileName, configMap))
	{
		return -1;
	}
	objects.push_back(Model(configMap["model"], objId++));
	std::vector<Animation> animations = {
		{configMap[animationNames[CUR_ANIM_IDLE]], &objects.back()},
		{configMap[animationNames[CUR_ANIM_JUMP]], &objects.back()},
		{configMap[animationNames[CUR_ANIM_WALK]], &objects.back()},
		{configMap[animationNames[CUR_ANIM_KICK]], &objects.back()},
	};
    Animator animator(&animations[CUR_ANIM_IDLE]);
	for (auto& obj : objects)
	{
		keyboardManager.addObserver(&obj);
	}
	
	glfwSetKeyCallback(window, key_callback);
	// glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	
	// objects[0].getModelState().setElder();
	objects[0].setObjectAsActive();
	glClearColor(.5f, .5f, .5f, 1.0f);
	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	bool animationEnd = false;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		keyboardManager.update();
		
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (objects[0].getModelState().m_animationChange || animator.isAnimationEnd(deltaTime))
		{
			animationEnd = false;
			animator.PlayAnimation(&animations[objects[0].getModelState().m_currentAnimation]);
			objects[0].getModelState().m_currentAnimation = CUR_ANIM_IDLE;
			objects[0].getModelState().setAnimationChangeStatus(false);
		}
		animator.UpdateAnimation(deltaTime);
		auto transforms = animator.GetFinalBoneMatrices();
		objects[0].setTransforms(transforms);
		objects[0].Draw();

		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}