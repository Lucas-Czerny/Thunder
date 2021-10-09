#include "td_pch.h"
#include "ImGuiConfig.h"

namespace Thunder { namespace Config
{
	void SetCustomTheme()
	{
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Text] = RGB_INT_TO_FLOAT(255, 255, 255, 1.0f);

		// Window
		colors[ImGuiCol_WindowBg] = RGB_INT_TO_FLOAT(20, 21, 23, 1.0f);

		// Headers
		colors[ImGuiCol_Header] = RGB_INT_TO_FLOAT(55, 58, 66, 0.5f);
		colors[ImGuiCol_HeaderHovered] = RGB_INT_TO_FLOAT(55, 58, 66, 0.3f);
		colors[ImGuiCol_HeaderActive] = RGB_INT_TO_FLOAT(55, 58, 66, 0.2f);

		// Buttons
		colors[ImGuiCol_Button] = RGB_INT_TO_FLOAT(55, 58, 66, 1.0f);
		colors[ImGuiCol_ButtonHovered] = RGB_INT_TO_FLOAT(55, 58, 66, 0.8f);
		colors[ImGuiCol_ButtonActive] = RGB_INT_TO_FLOAT(55, 58, 66, 0.7f);

		// Frame BG
		colors[ImGuiCol_FrameBg] = RGB_INT_TO_FLOAT(55, 58, 66, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = RGB_INT_TO_FLOAT(55, 58, 66, 0.8f);
		colors[ImGuiCol_FrameBgActive] = RGB_INT_TO_FLOAT(55, 58, 66, 0.7f);

		// Tabs
		colors[ImGuiCol_Tab] = RGB_INT_TO_FLOAT(114, 42, 222, 1.0f);
		colors[ImGuiCol_TabHovered] = RGB_INT_TO_FLOAT(114, 42, 222, 1.0f);
		colors[ImGuiCol_TabActive] = RGB_INT_TO_FLOAT(114, 42, 222, 1.0f);
		colors[ImGuiCol_TabUnfocused] = RGB_INT_TO_FLOAT(114, 42, 222, 1.0f);
		colors[ImGuiCol_TabUnfocusedActive] = RGB_INT_TO_FLOAT(114, 42, 222, 1.0f);

		// Title
		colors[ImGuiCol_TitleBg] = RGB_INT_TO_FLOAT(74, 28, 140, 1.0f);
		colors[ImGuiCol_TitleBgActive] = RGB_INT_TO_FLOAT(74, 28, 140, 1.0f);
		colors[ImGuiCol_TitleBgCollapsed] = RGB_INT_TO_FLOAT(74, 28, 140, 1.0f);

		// Other
		colors[ImGuiCol_CheckMark] = RGB_INT_TO_FLOAT(255, 255, 255, 1.0f);
	}
} }