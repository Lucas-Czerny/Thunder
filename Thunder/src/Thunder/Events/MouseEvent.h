#pragma once

#include "Event.h"

namespace Thunder
{
	class THUNDER_API MouseMovedEvent : public Event
	{
	private:
		float m_MouseX, m_MouseY;
		
	public:
		MouseMovedEvent(float mouseX, float mouseY)
			: m_MouseX(mouseX), m_MouseY(mouseY) {}

		std::string ToString() const override
		{
			std::stringstream result;
			result << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return result.str();
		}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(MouseCategory | InputCategory);
	};

	class THUNDER_API MouseScrolledEvent : public Event
	{
	private:
		float m_xOffset, m_yOffset;

	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_xOffset(xOffset), m_yOffset(yOffset) {}

		std::string ToString() const override
		{
			std::stringstream result;
			result << "MouseScrolledEvent: " << m_xOffset << ", " << m_yOffset;
			return result.str();
		}

		inline float GetXOffset() const { return m_xOffset; }
		inline float GetYOffset() const { return m_yOffset; }

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(MouseCategory | InputCategory);
	};

	class THUNDER_API MouseButtonEvent : public Event
	{
	protected:
		MouseButtonEvent(int mouseButton)
			: m_MouseButton(mouseButton) {}

		int m_MouseButton;

	public:
		inline int GetMouseButton() const { return m_MouseButton; }

		EVENT_CLASS_CATEGORY(MouseCategory | InputCategory);
	};

	class THUNDER_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int mouseButton)
			: MouseButtonEvent(mouseButton) {}

		std::string ToString() const override
		{
			std::stringstream result;
			result << "MouseButtonPressedEvent: " << m_MouseButton;
			return result.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed);
	};

	class THUNDER_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int mouseButton)
			: MouseButtonEvent(mouseButton) {}

		std::string ToString() const override
		{
			std::stringstream result;
			result << "MouseButtonReleasedEvent: " << m_MouseButton;
			return result.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);
	};
}