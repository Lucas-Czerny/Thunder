#include "td_pch.h"

#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include "Thunder/ImGui/ImGuiConfig.h"

namespace Thunder
{
	enum class DialogResponse
	{
		NONE = 0,
		YES, NO,
		OK, CANCEL
	};
}

namespace Thunder { namespace ImCustom
{
	static std::map<const char*, bool> DialogOpenMap;

	void CenterAlignedText(const char* text, float width)
	{
		float posX = ImGui::GetCursorPosX() + (width - ImGui::CalcTextSize(text).x) / 2;

		if (posX > ImGui::GetCursorPosX())
			ImGui::SetCursorPosX(posX);

		ImGui::Text(text);
	}

	void DrawVec3Control(const std::string& label, glm::vec3& values, const glm::vec3& resetValues /* = { 0.0f, 0.0f, 0.0f } */)
	{
		ImGui::PushID(label.c_str());

		float columnWidth = ImGui::CalcItemWidth() / 2;

		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

		{
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.2901f, 0.1098f, 0.5490f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.2901f, 0.1098f, 0.5490f, 0.85f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.2901f, 0.1098f, 0.5490f, 0.75f });

			if (ImGui::Button("X", { 23, 23 }))
				values.x = resetValues.x;

			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");

			ImGui::PopItemWidth();
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 5, 0 });
		}

		{
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, { 0.2901f, 0.1098f, 0.5490f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.2901f, 0.1098f, 0.5490f, 0.85f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.2901f, 0.1098f, 0.5490f, 0.75f });

			if (ImGui::Button("Y", { 23, 23 }))
				values.y = resetValues.y;

			ImGui::PopStyleVar();

			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");

			ImGui::PopItemWidth();
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 5, 0 });
		}

		{
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, { 0.2901f, 0.1098f, 0.5490f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.2901f, 0.1098f, 0.5490f, 0.85f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.2901f, 0.1098f, 0.5490f, 0.75f });

			if (ImGui::Button("Z", { 23, 23 }))
				values.z = resetValues.z;

			ImGui::PopStyleVar();

			ImGui::PopStyleColor(3);

			ImGui::SameLine();

			ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
		}

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);
	}

	void DrawPopup(const char* popupID, bool open, std::function<bool(void)> openCallback)
	{
		if (open)
			ImGui::OpenPopup(popupID);

		if (ImGui::BeginPopup(popupID))
		{
			if (openCallback())
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void DrawDialog(const char* popupID, std::function<void(void)> uiCode)
	{
		if (DialogOpenMap[popupID])
			ImGui::OpenPopup(popupID);

		if (ImGui::BeginPopupModal(popupID))
		{
			uiCode();

			ImGui::EndPopup();
		}
	}

	template <typename T>
	void OpenDialog(const char* popupID, std::function<void(DialogResponse, T)> callback)
	{
		DialogOpenMap[popupID] = true;
		DialogCallback<T> = callback;
	}

	template <typename T>
	void CloseDialog(const DialogResponse& response, T result)
	{
		ImGui::CloseCurrentPopup();

		DialogCallback<T>(response, result);
	}

	bool DragFloat(const char* label, float* value, const Config::DragFloatArguments& args)
	{
		return ImGui::DragFloat(label, value, args.Speed, args.Min, args.Max);
	}

	bool SliderInt(const char* label, int* value, const Config::SliderIntArguments& args)
	{
		return ImGui::SliderInt(label, value, args.Min, args.Max);
	}
} }