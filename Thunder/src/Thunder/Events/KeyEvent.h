#pragma once

#include "Event.h"

namespace Thunder
{
	class THUNDER_API KeyEvent : public Event
	{
	protected:
		KeyEvent(int keyCode)
			: m_KeyCode(keyCode) {}

		int m_KeyCode;

	public:
		inline int GetKeyCode() const { return m_KeyCode; }
		
		EVENT_CLASS_CATEGORY(KeyboardCategory | InputCategory);
	};

	class THUNDER_API KeyPressedEvent : public KeyEvent
	{
	private:
		unsigned int m_RepeatCount;

	public:
		KeyPressedEvent(int keyCode, int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		std::string ToString() const override
		{
			std::stringstream result;
			result << "KeyPressedEvent: " << m_KeyCode << "(" << m_RepeatCount << " repeats)";
			return result.str();
		}

		inline unsigned int GetRepeatCount() const { return m_RepeatCount; }

		EVENT_CLASS_TYPE(KeyPressed);
	};

	class THUNDER_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);
	};

	class THUNDER_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keyCode)
			: KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream result;
			result << "KeyPressedEvent: " << m_KeyCode;
			return result.str();
		}

		EVENT_CLASS_TYPE(KeyTyped);
	};
};