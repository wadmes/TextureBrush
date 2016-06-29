#pragma once

#include "../preHeader.h"
#include "../eventHandler/eventManager.h"

namespace TextureSynthesis
{

class CCamera;
class CViewer : public MouseListener, public KeyListener
{
public:
	static CViewer* getViewerInstance();
	virtual ~CViewer();

	void aim();

	bool keyPressed(const KeyEvent &arg);
	bool keyReleased(const KeyEvent &arg);

	bool mouseMoved(const MouseEvent &arg);
	bool mousePressed(const MouseEvent &arg, int id);
	bool mouseReleased(const MouseEvent &arg, int id);
	bool mouseWheel(int vOffset);

	void setSpeed(const float& zoom, const float& move, const float& tumbling)
	{
		m_zoomSpeed = zoom;
		m_moveSpeed = move;
		m_tumblingSpeed = tumbling;
	}

protected:
	CViewer();
	CCamera* m_pCamera;

private:
	float m_tumblingSpeed, m_zoomSpeed, m_moveSpeed;
	vec3  m_forward, m_up, m_side;

	int m_preMouseXPos;
	int m_preMouseYPos;

	int m_preWheelPos;

	bool m_isPaintingMode;
	bool m_mouseDown, m_startTumbling, m_zoomMode;
};

} // end namespace