#pragma once

#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include "Thunder/ImGui/ImGuiConfig.h"

namespace Thunder
{
	enum class DialogResponse;

	namespace ImCustom
	{
		inline void CenterAlignedText(const char* text, float width);

		inline void DrawVec3Control(const std::string& label, glm::vec3& values, const glm::vec3& resetValues = { 0.0f, 0.0f, 0.0f });

		inline void DrawPopup(const char* popupID, bool open, std::function<bool(void)> openCallback);

		template <typename T>
		static std::function<void(DialogResponse, T)> DialogCallback;

		inline void DrawDialog(const char* popupID, std::function<void(void)> uiCode);

		template <typename T>
		inline void OpenDialog(const char* popupID, std::function<void(DialogResponse, T)> callback);

		template <typename T>
		inline void CloseDialog(const DialogResponse& response, T result);

		inline bool DragFloat(const char* label, float* value, const Config::DragFloatArguments& args);

		inline bool SliderInt(const char* label, int* value, const Config::SliderIntArguments& args);
	};
}

#include "CustomImGuiWidgets.inl"
