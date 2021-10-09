#include "td_pch.h"
#include "EditorCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Thunder/Core/KeyCodes.h"
#include "Thunder/Core/MouseButtonCodes.h"

namespace Thunder
{
	EditorCamera::EditorCamera(float width, float height, float nearPlane /* = 0.001f */, float farPlane /* = 10000.0f */, float verticalFOV /* = glm::radians(30.0f) */)
	{
		m_AspectRatio = width / height;
		m_Near = nearPlane, m_Far = farPlane;
		m_VerticalFOV = verticalFOV;

		RecalculateViewProjectionMatrix();
	}

	void EditorCamera::OnUpdate(Timestep deltaTime)
	{
		bool controlModifier = Input::GetKeyDown(TD_KEY_LEFT_CONTROL) | Input::GetKeyDown(TD_KEY_RIGHT_CONTROL);

		glm::vec2 mousePosition = { Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 mouseDelta = mousePosition - m_OldMousePosition;

		if (controlModifier && Input::GetMouseButtonPressed(TD_MOUSE_BUTTON_MIDDLE))
			Drag(mouseDelta * (float)deltaTime * m_DragSpeed);

		if (controlModifier && Input::GetMouseButtonPressed(TD_MOUSE_BUTTON_RIGHT))
			Rotate(mouseDelta * (float)deltaTime * m_RotationSpeed);

		m_OldMousePosition = { Input::GetMouseX(), Input::GetMouseY() };
	}

	void EditorCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(TD_BIND_EVENT_FUNCTION(EditorCamera::OnMouseScrolled));
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& event)
	{
		bool controlModifier = Input::GetKeyPressed(TD_KEY_LEFT_CONTROL) | Input::GetKeyPressed(TD_KEY_RIGHT_CONTROL);
		if (!controlModifier || Input::GetMouseButtonPressed(TD_MOUSE_BUTTON_MIDDLE))
			return false;

		float offset = event.GetYOffset();
		m_ZoomSpeed = glm::length(m_Position) * 3.5f;
		Zoom(offset * m_ZoomSpeed);

		return true;
	}

	void EditorCamera::Drag(const glm::vec2& mouseDelta)
	{
		m_Position += -mouseDelta.x * GetRightDirection();
		m_Position += mouseDelta.y * GetUpDirection();

		RecalculateViewProjectionMatrix();
	}

	void EditorCamera::Zoom(float offset)
	{
		glm::vec3 forwardDirection = GetForwardDirection() * 0.01f;
		m_Position += forwardDirection * offset;

		RecalculateViewProjectionMatrix();
	}

	void EditorCamera::Rotate(const glm::vec2& mouseDelta)
	{
		m_Rotation.x += -mouseDelta.y;
		m_Rotation.y += -mouseDelta.x;

		RecalculateViewProjectionMatrix();
	}

	void EditorCamera::SetViewportSize(float width, float height)
	{
		m_AspectRatio = width / height;
		RecalculateViewProjectionMatrix();
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(glm::quat(m_Rotation), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(glm::quat(m_Rotation), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(glm::quat(m_Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void EditorCamera::RecalculateViewProjectionMatrix()
	{
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position)
			* glm::toMat4(glm::quat(m_Rotation));

		m_ViewMatrix = glm::inverse(m_ViewMatrix);

		m_ProjectionMatrix = glm::perspective(m_VerticalFOV, m_AspectRatio, m_Near, m_Far);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}