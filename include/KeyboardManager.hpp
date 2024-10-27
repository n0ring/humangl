#pragma once

#include <unordered_map>
#include <vector>
#include "Model.hpp"
#define KEY_SHIFT GLFW_KEY_RIGHT

class KeyboardManager
{
public:
	KeyboardManager() {}
	void update();
	void setKeyState(int key, int action);

	void addObserver(Model* p_object);

	void spellCheck();
private:
	std::unordered_map<int, int> keysState = {
		{GLFW_KEY_RIGHT, 0},
		{GLFW_KEY_LEFT, 0},
		{GLFW_KEY_DOWN, 0},
		{GLFW_KEY_UP, 0},
		{GLFW_KEY_EQUAL, 0},
		{GLFW_KEY_LEFT_SHIFT, 0},
		{GLFW_KEY_MINUS, 0},
		{GLFW_KEY_F, 0},
		{GLFW_KEY_U, 0},
		{GLFW_KEY_W, 0},
		{GLFW_KEY_S, 0},
		{GLFW_KEY_D, 0},
		{GLFW_KEY_A, 0},
		{GLFW_KEY_E, 0},
		{GLFW_KEY_Q, 0},
		{GLFW_KEY_T, 0},
		{GLFW_KEY_C, 0},
		{GLFW_KEY_1, 0},
		{GLFW_KEY_2, 0},
		{GLFW_KEY_L, 0},
		{GLFW_KEY_M, 0},
		{GLFW_KEY_O, 0},
		{GLFW_KEY_N, 0},
		{GLFW_KEY_O, 0},
		{GLFW_KEY_X, 0},
		{GLFW_KEY_SPACE, 0},
		{GLFW_KEY_G, 0},
		{GLFW_KEY_ENTER, 0}
	};

	std::vector<Model*> m_observers;
	int m_activeObject = 0; // 0 object is elder
	std::string kLumos = "lumos";
	std::string kNox = "nox";
	int m_lumosIdx = 0;
	int m_noxIdx = 0;
};


// #define GLFW_KEY_RIGHT              262
// #define GLFW_KEY_LEFT               263
// #define GLFW_KEY_DOWN               264
// #define GLFW_KEY_UP                 265