#pragma once

#include <glm/glm.hpp>

#include "Thunder/Core/Core.h"

namespace Thunder {	namespace Input
{
	bool GetKeyDown(int keycode);
	bool GetKeyPressed(int keycode);

	bool Shortcut();

	template <class... T>
	bool Shortcut(int keycode, const T&... rest) {
		if (!GetKeyPressed(keycode)) return false;

		return Shortcut(rest...);
	}

	bool GetMouseButtonPressed(int button);

	glm::vec2 GetMousePosition();

	float GetMouseX();
	float GetMouseY();
} }