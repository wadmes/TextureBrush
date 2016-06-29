#include "eventManager.h"

using namespace TextureSynthesis;

CEventManager* CEventManager::getEventManagerInstance()
{
	static CEventManager *s_pEventManager = NULL;

	if (s_pEventManager == NULL)
	{
		s_pEventManager = new CEventManager();
	}

	return s_pEventManager;
}

CEventManager::CEventManager() :m_wheelPos(0)
{
	int totalKCNum = GLFW_KEY_LAST;
	for (int idx = 0; idx < totalKCNum; ++idx)
	{
		m_keyPressed[idx] = false;
		m_pKeyListeners[idx] = NULL;
	}

	int totalMCNum = GLFW_MOUSE_BUTTON_LAST - GLFW_MOUSE_BUTTON_1 + 1;
	for (int idx = 0; idx < totalMCNum; ++idx)
	{
		m_mousePressed[idx] = false;
		m_pMouseListener[idx] = NULL;
	}
}

void CEventManager::registerKeyListener(int keyCode, KeyListener* pListener)
{
	int keyCodeOffset = keyCode - 1;
	if (m_pKeyListeners[keyCodeOffset] != NULL)
	{
		cout << "WARNING: Key listener registered to a registered key!" << endl;
	}
	else
	{
		m_pKeyListeners[keyCodeOffset] = pListener;
	}
}

void CEventManager::registerMouseListener(int mouseCode, MouseListener* pListener)
{
	int mouseCodeOffset = mouseCode;
	if (m_pMouseListener[mouseCodeOffset] != NULL)
	{
		cout << "WARNING: Key listener registered to a registered key!" << endl;
	}
	else
	{
		m_pMouseListener[mouseCodeOffset] = pListener;
	}
}

void CEventManager::keyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
	int keyCodeOffset = key - 1;
	if (action == GLFW_PRESS)
	{
		if (getEventManagerInstance()->m_pKeyListeners[keyCodeOffset] != NULL)
		{
			getEventManagerInstance()->m_pKeyListeners[keyCodeOffset]->keyPressed(KeyEvent(key));
		}

		getEventManagerInstance()->m_keyPressed[keyCodeOffset] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		if (getEventManagerInstance()->m_pKeyListeners[keyCodeOffset] != NULL)
		{
			getEventManagerInstance()->m_pKeyListeners[keyCodeOffset]->keyReleased(KeyEvent(key));
		}

		getEventManagerInstance()->m_keyPressed[keyCodeOffset] = false;
	}
}

void CEventManager::mouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (getEventManagerInstance()->m_pMouseListener[button] != NULL)
		{
			getEventManagerInstance()->m_pMouseListener[button]->mousePressed(MouseEvent(MouseState(button)), button);
		}
	}
	else if (action == GLFW_RELEASE)
	{
		if (getEventManagerInstance()->m_pMouseListener[button] != NULL)
		{
			getEventManagerInstance()->m_pMouseListener[button]->mouseReleased(MouseEvent(MouseState(button)), button);
		}
	}
}

void CEventManager::mousePosCallback(GLFWwindow* pWindow, double x, double y)
{
	for (int mouseIdx = 0; mouseIdx <= GLFW_MOUSE_BUTTON_LAST - GLFW_MOUSE_BUTTON_1; ++mouseIdx)
	{
		if (getEventManagerInstance()->m_pMouseListener[mouseIdx] != NULL)
		{
			getEventManagerInstance()->m_pMouseListener[mouseIdx]->mouseMoved(MouseEvent(MouseState(x, y)));
		}
	}
}

void CEventManager::mouseWheelCallback(GLFWwindow* pWindow, double x, double y)
{
	for (int mouseIdx = 0; mouseIdx <= GLFW_MOUSE_BUTTON_LAST - GLFW_MOUSE_BUTTON_1; ++mouseIdx)
	{
		if (getEventManagerInstance()->m_pMouseListener[mouseIdx] != NULL)
		{
			getEventManagerInstance()->m_pMouseListener[mouseIdx]->mouseWheel(y);
		}
	}
}