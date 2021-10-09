#include "td_pch.h"
#include "OrthographicCameraController.h"

#include <glm/gtx/rotate_vector.hpp>

#include "Thunder/Core/Input.h"
#include "Thunder/Core/KeyCodes.h"
#include "Thunder/Renderer/Renderer.h"

namespace Thunder
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool enableRotation /* = false */)
		: m_Camera({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_AspectRatio(aspectRatio), m_EnableRotation(enableRotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep deltaTime)
	{
		TD_PROFILE_FUNCTION();

		if (m_HoldingLeftMouseButton)
		{
			glm::vec2 mouseMovementDirection = m_MousePosition - m_OldMousePosition;
	
			m_Position.x -= mouseMovementDirection.x * m_MovementSpeed * deltaTime;
			m_Position.y += mouseMovementDirection.y * m_MovementSpeed * deltaTime;
		}

		if (m_EnableRotation)
		{
			if (Thunder::Input::GetKeyDown(TD_KEY_E))
				m_Rotation += m_RotationSpeed * deltaTime;
			else if (Thunder::Input::GetKeyDown(TD_KEY_Q))
				m_Rotation += -m_RotationSpeed * deltaTime;

			m_Camera.SetRotation(m_Rotation);
		}

		m_Camera.SetPosition(m_Position);

		m_OldMousePosition = m_MousePosition;
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<MouseScrolledEvent>(TD_BIND_EVENT_FUNCTION(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(TD_BIND_EVENT_FUNCTION(OrthographicCameraController::OnWindowResized));
		dispatcher.Dispatch<MouseMovedEvent>(TD_BIND_EVENT_FUNCTION(OrthographicCameraController::OnMouseMoved));
		dispatcher.Dispatch<MouseButtonPressedEvent>(TD_BIND_EVENT_FUNCTION(OrthographicCameraController::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(TD_BIND_EVENT_FUNCTION(OrthographicCameraController::OnMouseButtonReleased));
	}

	void OrthographicCameraController::SetZoomLevel(float zoomLevel)
	{
		m_ZoomLevel = std::clamp(zoomLevel, m_MinZoomLevel, m_MaxZoomLevel);
		m_MovementSpeed = m_ZoomLevel * 0.135f;

		RecalculateProjectionMatrix();
	}

	void OrthographicCameraController::Resize(float width, float height)
	{
		m_AspectRatio = width / height;
		RecalculateProjectionMatrix();
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_ZoomLevel -= event.GetYOffset() * m_ZoomLevel / 5;
		m_ZoomLevel = std::clamp(m_ZoomLevel, m_MinZoomLevel, m_MaxZoomLevel);

		m_MovementSpeed = m_ZoomLevel * 0.135f;

		RecalculateProjectionMatrix();

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		Resize((float)event.GetWidth(), (float)event.GetHeight());

		return false;
	}

	bool OrthographicCameraController::OnMouseMoved(MouseMovedEvent& event)
	{
		m_MousePosition = { event.GetX(), event.GetY() };

		return false;
	}

	bool OrthographicCameraController::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		if (event.GetMouseButton() == 0)
			m_HoldingLeftMouseButton = true;

		return false;
	}

	bool OrthographicCameraController::OnMouseButtonReleased(MouseButtonReleasedEvent& event)
	{
		if (event.GetMouseButton() == 0)
			m_HoldingLeftMouseButton = false;

		return false;
	}

	void OrthographicCameraController::RecalculateProjectionMatrix()
	{
		m_Camera.SetProjectionMatrix({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel });
	}
}