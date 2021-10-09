#pragma once

#include <Thunder.h>

using namespace Thunder;

class CameraController : public ScriptableComponent
{
private:
	float zoomLevel = 2.0f;

	float movementSpeed = 0.1f;
	float zoomSpeed = 0.2f;

	bool holdingLeftMouseButton = false;
	glm::vec2 mousePosition = { 0.0f, 0.0f };
	glm::vec2 oldMousePosition = { 0.0f, 0.0f };

public:
	void OnCreate()
	{
		GetComponent<CameraComponent>().Camera.SetOrthographicSize(zoomLevel);
	}

	void OnUpdate(Timestep deltaTime)
	{
		if (holdingLeftMouseButton && !Input::GetMouseButtonPressed(TD_MOUSE_BUTTON_1))
			holdingLeftMouseButton = false;

		if (holdingLeftMouseButton)
		{
			glm::vec2 mouseMovementDirection = mousePosition - oldMousePosition;

			auto& transformComponent = GetComponent<TransformComponent>();

			transformComponent.Translation.x += mouseMovementDirection.x * movementSpeed * deltaTime;
			transformComponent.Translation.y -= mouseMovementDirection.y * movementSpeed * deltaTime;
		}

		oldMousePosition = mousePosition;
		mousePosition = { Input::GetMouseX(), Input::GetMouseY() };
	}

	void OnEvent(Event& event)
	{
		if (GetComponent<CameraComponent>().Camera.GetProjectionType() == ProjectionType::Orthographic)
		{
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MouseButtonPressedEvent>(TD_BIND_EVENT_FUNCTION(CameraController::OnMouseButtonPressed));
			dispatcher.Dispatch<MouseButtonReleasedEvent>(TD_BIND_EVENT_FUNCTION(CameraController::OnMouseButtonReleased));
			dispatcher.Dispatch<MouseScrolledEvent>(TD_BIND_EVENT_FUNCTION(CameraController::OnMouseScrolled));
		}
	}

	bool OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		if (event.GetMouseButton() == TD_MOUSE_BUTTON_1)
			holdingLeftMouseButton = true;

		return true;
	}

	bool OnMouseButtonReleased(MouseButtonReleasedEvent& event)
	{
		if (event.GetMouseButton() == TD_MOUSE_BUTTON_1)
			holdingLeftMouseButton = false;

		return true;
	}

	bool OnMouseScrolled(MouseScrolledEvent& event)
	{
		float offset = event.GetYOffset();
		zoomLevel -= offset * zoomLevel * zoomSpeed;

		zoomLevel = std::clamp(zoomLevel, 1.0f, 10.0f);
		movementSpeed = zoomLevel * 0.05f;

		GetComponent<CameraComponent>().Camera.SetOrthographicSize(zoomLevel);

		return true;
	}
};