#pragma once

#include "Thunder/Renderer/OrthographicCamera.h"

#include "Thunder/Core/Timestep.h"

#include "Thunder/Events/ApplicationEvent.h"
#include "Thunder/Events/MouseEvent.h"

namespace Thunder
{
	class OrthographicCameraController
	{
	private:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;

		float m_ZoomLevel = 1.0f;
		float m_MinZoomLevel = 0.25f, m_MaxZoomLevel = 10.0f;

		float m_AspectRatio;
		float m_EnableRotation;

		float m_MovementSpeed = 0.1f;
		float m_RotationSpeed = 180.0f;

		bool m_HoldingLeftMouseButton = false;
		glm::vec2 m_MousePosition = { 0.0f, 0.0f };
		glm::vec2 m_OldMousePosition = { 0.0f, 0.0f };

		OrthographicCamera m_Camera;

	public:
		OrthographicCameraController(float aspectRatio, bool enableRotation = false);

		void OnUpdate(Timestep deltaTime);
		void OnEvent(Event& event);

		void SetZoomLevel(float zoomLevel);
		void Resize(float width, float height);

		OrthographicCamera GetCamera() const { return m_Camera; }

	private:
		void RecalculateProjectionMatrix();

		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& event);
	};
}


