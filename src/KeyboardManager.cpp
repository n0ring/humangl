#include "KeyboardManager.hpp"

void KeyboardManager::update()
{
	for (auto& [key, state] : keysState)
	{
		if (state == 3 && key == GLFW_KEY_G)
		{
			state = 0;
			m_observers[m_activeObject]->getModelState().setAnimationChangeStatus(true);
		}
		if (state == GLFW_PRESS)
		{
			switch (key)
			{
				case GLFW_KEY_T:
					m_observers[m_activeObject]->getModelState().toggleTexColor();
					keysState[GLFW_KEY_T] = 0;
					break;
				case GLFW_KEY_1:
					m_observers[m_activeObject]->setObjectAsNotActive();
					m_activeObject--;
					if (m_activeObject < 0)
						m_activeObject = m_observers.size() - 1;
					m_observers[m_activeObject]->setObjectAsActive();
					keysState[GLFW_KEY_1] = 0;
					break;
				case GLFW_KEY_2:
					m_observers[m_activeObject]->setObjectAsNotActive();
					m_activeObject++;
					if (m_activeObject >= m_observers.size())
						m_activeObject = 0;
					keysState[GLFW_KEY_2] = 0;
					m_observers[m_activeObject]->setObjectAsActive();
					break;
				case GLFW_KEY_SPACE: 
					keysState[GLFW_KEY_SPACE] = 0;
					m_observers[m_activeObject]->getModelState().showNextBone();
					m_observers[m_activeObject]->getModelState().changeAnimToJump();
					m_observers[m_activeObject]->getModelState().setAnimationChangeStatus(true);
					break;
				case GLFW_KEY_ENTER:
					keysState[GLFW_KEY_ENTER] = 0;
					m_observers[m_activeObject]->getModelState().changeAnimToKick();
					m_observers[m_activeObject]->getModelState().setAnimationChangeStatus(true);
					break;

			}
		}
		if (state == GLFW_PRESS || state == GLFW_REPEAT)
		{
			switch (key)
			{
			case GLFW_KEY_UP:
				m_observers[m_activeObject]->getModelState().moveUp();
				break;
			case GLFW_KEY_DOWN:
				m_observers[m_activeObject]->getModelState().moveDown();
				break;
			case GLFW_KEY_LEFT:
				m_observers[m_activeObject]->getModelState().moveLeft();
				break;
			case GLFW_KEY_RIGHT:
				m_observers[m_activeObject]->getModelState().moveRight();
				break;
			case GLFW_KEY_MINUS:
				m_observers[m_activeObject]->getModelState().decreaseScale();
				break;
			case GLFW_KEY_EQUAL:
				if (keysState[GLFW_KEY_LEFT_SHIFT] > 0)
					m_observers[m_activeObject]->getModelState().increaseScale();
				break;
			case GLFW_KEY_F:
				m_observers[m_activeObject]->getModelState().fill_model = true;
				break;
			case GLFW_KEY_U:
				m_observers[m_activeObject]->getModelState().fill_model = false;
				break;
			case GLFW_KEY_W:
				m_observers[m_activeObject]->getModelState().moveModelUp();
				break;
			case GLFW_KEY_S:
				m_observers[m_activeObject]->getModelState().moveModelDown();
				break;
			case GLFW_KEY_D:
				m_observers[m_activeObject]->getModelState().moveModelRight();
				break;
			case GLFW_KEY_A:
				m_observers[m_activeObject]->getModelState().moveModelLeft();
				break;
			case GLFW_KEY_E:
				m_observers[m_activeObject]->getModelState().moveModelFar();
				break;
			case GLFW_KEY_Q:
				m_observers[m_activeObject]->getModelState().moveModelClose();
				break;
			case GLFW_KEY_G:
				m_observers[m_activeObject]->getModelState().changeAnimToWalk();
				m_observers[m_activeObject]->getModelState().setAnimationChangeStatus(true);
				break;
			default:
				break;
			}
		}
	}
}

void KeyboardManager::spellCheck()
{
		for (auto [key, state] : keysState)
	{
		if (state == GLFW_PRESS || state == GLFW_REPEAT)
		{
			switch (key)
			{
			case GLFW_KEY_U:
				if (kLumos[m_lumosIdx] == 'u')
					m_lumosIdx++;
				break;
			case GLFW_KEY_S:
				if (kLumos[m_lumosIdx] == 's')
					m_lumosIdx++;
				break;
			case GLFW_KEY_L:
				if (kLumos[m_lumosIdx] == 'l')
					m_lumosIdx++;
				break;
			case GLFW_KEY_N:
				if (kNox[m_noxIdx] == 'n')
					m_noxIdx++;
				break;
			case GLFW_KEY_M:
				if (kLumos[m_lumosIdx] == 'm')
					m_lumosIdx++;
				break;
			case GLFW_KEY_O:
				if (kLumos[m_lumosIdx] == 'o')
					m_lumosIdx++;
				if (kNox[m_noxIdx] == 'o')
					m_noxIdx++;
				break;
			case GLFW_KEY_X:
				if (kNox[m_noxIdx] == 'x')
					m_noxIdx++;
				break;
			default:
				break;
			}
		}
	}
	if (m_lumosIdx == kLumos.size())
	{
		for (auto& obj : m_observers)	
			obj->getModelState().lumos();
		m_lumosIdx = 0;
	}
	if (m_noxIdx == kNox.size())
	{
		for (auto& obj : m_observers)	
			obj->getModelState().nox();
		m_noxIdx = 0;
	}
}


void KeyboardManager::setKeyState(int key, int action)
{
	if (keysState.count(key))
	{
		if (key == GLFW_KEY_G && action == 0)
		{
			keysState[key] = 3;
		}
		else 
			keysState[key] = action;
	}
}

void KeyboardManager::addObserver(Model *p_object)
{
	m_observers.push_back(p_object);
}
