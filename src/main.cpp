#include <iostream>
#include <vector>

#include "OpenGlSetter.hpp"
#include "KeyboardManager.hpp"

// #include "Model.hpp"
#include "Animator.hpp"
#include "AnimationConfigParser.hpp"

std::string animationNames[] = {"idle", "jump", "walk", "kick"};
KeyboardManager g_keyboardManager;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	g_keyboardManager.setKeyState(key, action);
}

int main(int argc, char **argv)
{
	std::vector<Model> objects;
	std::unordered_map<std::string, std::string> configMap;
	std::string configFileName;
	float deltaTime = 0.0f;

	if (argc != 2)
	{
		std::cout << "Usage of humangl: \n";
		std::cout << "For render model with animations set simple config file and give the path to config file to app. Example: ./humangl pathToConfigfile\n";
		std::cout << "Controls: \n";
		std::cout << "WASD for move object in xy axis\n";
		std::cout << "Q E  for move object in z axis\n";
		std::cout << "UP DOWN LEFT RIGHT  for turn object\n";
		std::cout << "T for toggle between texture color modes\n";
		std::cout << "F for lines only. U for fill mode\n";
		std::cout << "- + for scale object (use left shit for +)\n";
		std::cout << "Animations: \n";
		std::cout << "Idle works as named \n";
		std::cout << "G for walking \n";
		std::cout << "SPACE for jump for walking \n";
		std::cout << "ENTER for kick \n";
		std::cout << "Config file example: \n";
		std::cout<< "	model = res/model/model.dae" << std::endl;
		std::cout<< "	idle = res/model/idle.dae" << std::endl;
		std::cout<< "	jump = res/model/jump.dae" << std::endl;
		std::cout<< "	walk = res/model/walk.dae" << std::endl;
		std::cout<< "	kick = res/model/kick.dae" << std::endl;
		return -1;
	}
	configFileName = argv[1];
	OpenGlSetter openglObj;
	if (openglObj.getState() == false)
	{
		std::cout << "opengl init error\n";
		return  - 1;
	}
	openglObj.setKeyboardCallback(key_callback);

	if (!AnimationConfigParser::parseConfig(configFileName, configMap))
	{
		return -1;
	}
	objects.push_back(Model(configMap["model"], 0));

	std::vector<Animation> animations = {
		{configMap[animationNames[CUR_ANIM_IDLE]], &objects.back()},
		{configMap[animationNames[CUR_ANIM_JUMP]], &objects.back()},
		{configMap[animationNames[CUR_ANIM_WALK]], &objects.back()},
		{configMap[animationNames[CUR_ANIM_KICK]], &objects.back()},
	};
	Animator animator(&animations[CUR_ANIM_IDLE]);

	g_keyboardManager.addObserver(&objects[0]);
	objects[0].setObjectAsActive();
	
	while (openglObj.isAppRunning())
	{
		openglObj.clearScreen();
		g_keyboardManager.update();
		deltaTime = openglObj.getDeltaTime();

		if (objects[0].getModelState().m_animationChange || animator.isAnimationEnd(deltaTime))
		{
			animator.PlayAnimation(&animations[objects[0].getModelState().m_currentAnimation]);
			objects[0].getModelState().m_currentAnimation = CUR_ANIM_IDLE;
			objects[0].getModelState().setAnimationChangeStatus(false);
		}
		animator.UpdateAnimation(deltaTime);
		auto transforms = animator.GetFinalBoneMatrices();
		objects[0].setTransforms(transforms);
		objects[0].Draw();

		openglObj.swapBuffersPollEvents();
	}
	return 0;
}