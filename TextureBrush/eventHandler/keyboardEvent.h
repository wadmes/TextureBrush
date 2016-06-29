#pragma once

#include "../preHeader.h"

namespace TextureSynthesis
{
	class KeyEvent
	{
	public:
		KeyEvent(int kc) : key(kc){}
		virtual ~KeyEvent() {}

		//! KeyCode of event
		const int key;
	};

	class KeyListener
	{
	public:
		virtual ~KeyListener() {}
		virtual bool keyPressed(const KeyEvent &arg) = 0;
		virtual bool keyReleased(const KeyEvent &arg) = 0;
	};
}