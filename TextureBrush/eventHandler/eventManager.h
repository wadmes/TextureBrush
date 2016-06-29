#pragma once

#include "../preHeader.h"
#include "keyboardEvent.h"
#include "mouseEvent.h"

namespace TextureSynthesis
{

class CEventManager
{
public:
	static CEventManager* getEventManagerInstance();
	virtual ~CEventManager(){}

public:
	void registerKeyListener(int keyCode, KeyListener* pListener);
	void registerMouseListener(int mouseCode, MouseListener* pListener);

	static void keyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods);
	static void mousePosCallback(GLFWwindow* pWindow, double x, double y);
	static void mouseWheelCallback(GLFWwindow* pWindow, double x, double y);

protected:
	CEventManager();

private:
	bool m_keyPressed[GLFW_KEY_LAST];
	KeyListener* m_pKeyListeners[GLFW_KEY_LAST];

	bool m_mousePressed[GLFW_MOUSE_BUTTON_LAST - GLFW_MOUSE_BUTTON_1];
	MouseListener* m_pMouseListener[GLFW_MOUSE_BUTTON_LAST - GLFW_MOUSE_BUTTON_1];

	int m_wheelPos;
};

}