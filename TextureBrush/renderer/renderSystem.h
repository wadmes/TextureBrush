#pragma once

#include "../preHeader.h"
#include "../eventHandler/eventManager.h"

namespace TextureSynthesis
{

class CRenderSystem : public KeyListener
{
public:
	static CRenderSystem* Instance();
	virtual ~CRenderSystem();

	GLFWwindow* getGLFWWindow(){ return m_pMainWindow; }

protected:
	CRenderSystem();

public:
	void initRenderSystem();

	void initGlew();
	void initGlfw();
	void initDevIL();

	void setEventCallbackFuncs();

	void createRenderWindow();

	void cleanSystem();

	void render();

	// Event handler
	bool keyPressed(const KeyEvent &arg);
	bool keyReleased(const KeyEvent &arg);

private:
	string								m_winTitle;
	GLFWwindow*							m_pMainWindow;
};

} // End of namespace