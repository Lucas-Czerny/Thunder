#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Thunder/Core/Timestep.h"
#include "Thunder/Core/Input.h"

#include "Thunder/Events/Event.h"
#include "Thunder/Events/MouseEvent.h"

#include "Thunder/Renderer/Camera.h"

namespace Thunder
{
	class EditorCamera : public Camera
	{
	private:
		glm::vec3 m_Position{ 0.0f, 0.0f, 3.0f };
		glm::vec3 m_Rotation{ 0.0f, 0.0f, 0.0f };

		float m_DragSpeed = 0.3f;
		float m_RotationSpeed = 0.05f;
		float m_ZoomSpeed = 20.0f;

		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		float m_AspectRatio;
		float m_Near, m_Far;
		float m_VerticalFOV;

		glm::vec2 m_OldMousePosition{ 0.0f, 0.0f };

	public:
		EditorCamera(float width, float height, float nearPlane = 0.001f, float farPlane = 10000.0f, float verticalFOV = glm::radians(30.0f));

		void OnUpdate(Timestep deltaTime);
		void OnEvent(Event& event);

		void Drag(const glm::vec2& mouseDelta);
		void Zoom(float offset);
		void Rotate(const glm::vec2& mouseDelta);

		void SetViewportSize(float width, float height);

		glm::vec3 GetForwardDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetUpDirection() const;

		glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }

	private:
		void RecalculateViewProjectionMatrix();

		bool OnMouseScrolled(MouseScrolledEvent& event);
	};
}