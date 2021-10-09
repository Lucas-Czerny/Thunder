#include "td_pch.h"

#ifdef TD_PLATFORM_WINDOWS

#include "Thunder/Core/Input.h"

#include <GLFW/glfw3.h>

#include "Thunder/Core/Application.h"

namespace Thunder
{
	bool Input::GetKeyDown(int keycode)
	{
		Application& app = Application::Get();
		GLFWwindow* window = (GLFWwindow*)app.GetWindow().GetNativeWindow();

		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::GetKeyPressed(int keycode)
	{
		Application& app = Application::Get();
		GLFWwindow* window = (GLFWwindow*)app.GetWindow().GetNativeWindow();

		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS;
	}

	bool Input::GetMouseButtonPressed(int button)
	{
		Application& app = Application::Get();
		GLFWwindow* window = (GLFWwindow*)app.GetWindow().GetNativeWindow();

		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	float Input::GetMouseX()
	{
		glm::vec2 mousePosition = GetMousePosition();

		return mousePosition.x;
	}

	float Input::GetMouseY()
	{
		glm::vec2 mousePosition = GetMousePosition();

		return mousePosition.y;
	}

	glm::vec2 Input::GetMousePosition()
	{
		Application& app = Application::Get();
		GLFWwindow* window = (GLFWwindow*)app.GetWindow().GetNativeWindow();

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return glm::vec2((float)xPos, (float)yPos);
	}
}

#endif