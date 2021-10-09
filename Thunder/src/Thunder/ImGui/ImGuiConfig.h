#pragma once

#include <imgui.h>

#include <glm/glm.hpp>

#define RGB_INT_TO_FLOAT(x, y, z, a) ImVec4(x / 255.0f, y / 255.0f, z / 255.0f, a)

namespace Thunder { namespace Config
{	
	struct DragFloatArguments
	{
		float Speed = 1.0f;
		float Min = 0.0f;
		float Max = 0.0f;
	};

	struct SliderIntArguments
	{
		int Min = 0;
		int Max = 0;
	};

	// EditorLayer

	constexpr glm::vec4 ClearColor = { 0.0784f, 0.0823f, 0.0901f, 1.0f };
	constexpr glm::vec2 DockspaceWindowMinSize = { 350.0f, 250.0f };

	constexpr SliderIntArguments PhysicsIterationSlider = { 0, 20 };

	// PropertiesPanel

	constexpr float AddComponentButtonWidth = 116.0f;
	constexpr float AddComponentPopupPaddingY = 5.0f;

	constexpr DragFloatArguments OrthographicSizeSliderArgs = { 0.1f, 0.001f, 20.0f };
	constexpr DragFloatArguments OrthographicNearSliderArgs = { 0.05f, -20.0f, 20.0f };
	constexpr DragFloatArguments OrthographicFarSliderArgs = { 0.05f, -20.0f, 20.0f };

	constexpr DragFloatArguments PerspectiveFOVSliderArgs = { 0.1f, 20.0f, 89.0f };
	constexpr DragFloatArguments PerspectiveNearSliderArgs = { 0.05f, 0.001f, 10000.0f };
	constexpr DragFloatArguments PerspectiveFarSliderArgs = { 0.05f, 1.0f, 10000.0f };

	constexpr DragFloatArguments PhysicsSliderArgs = { 0.05f, 0.0f, 1.0f };

	// FileBrowser

	constexpr glm::vec2 FileBrowserPadding = { 20.0f, 10.0f };
	
	constexpr float FileIconSize = 100.0f;

	// PlayPanel

	constexpr float PlayButtonSize = 40.0f;

	// ImGuiLayer

	void SetCustomTheme();
} }