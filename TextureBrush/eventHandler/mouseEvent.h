#pragma once

namespace TextureSynthesis
{
	class MouseState
	{
	public:
		MouseState() : xPos(0), yPos(0), button(0) {};
		MouseState(int vButton) : xPos(0), yPos(0), button(vButton) {};
		MouseState(int vX, int vY, int vButton = 0) : xPos(vX), yPos(vY), button(vButton) {};

		int xPos, yPos;
		int button;
	};

	class MouseEvent
	{
	public:
		MouseEvent(const MouseState &vState) : state(vState) {}
		virtual ~MouseEvent() {}

		const MouseState &state;
	};

	class MouseListener
	{
	public:
		virtual ~MouseListener() {}
		virtual bool mouseMoved(const MouseEvent &arg) = 0;
		virtual bool mousePressed(const MouseEvent &arg, int id) = 0;
		virtual bool mouseReleased(const MouseEvent &arg, int id) = 0;
		virtual bool mouseWheel(int vPos) = 0;
	};
}