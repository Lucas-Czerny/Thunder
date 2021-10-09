#include "td_pch.h"
#include "PlayPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Thunder/ImGui/ImGuiConfig.h"

namespace Thunder
{
	enum IconIndices
	{
		RuntimeIndex, RuntimeDebugIndex, StopIndex
	};

	PlayPanel::PlayPanel() :
		m_State(SceneState::Editor)
	{
		m_Textures[RuntimeIndex] = Texture2D::Create("resources/icons/runtime_play.png");
		m_Textures[RuntimeDebugIndex] = Texture2D::Create("resources/icons/runtime_debug.png");
		m_Textures[StopIndex] = Texture2D::Create("resources/icons/stop.png");
	}

	void PlayPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse;

		ImGui::BeginChild("Play", ImVec2{ -1, 40.0f }, false, flags);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

		switch (m_State)
		{
		case SceneState::Start:
			m_State = SceneState::Runtime;
		case SceneState::Stop:
			m_State = SceneState::Editor;
		}

		if (m_State != SceneState::Runtime)
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		float size = Config::PlayButtonSize;

		float combinedWidth = size * 3 + 16.0f;

		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - combinedWidth) * 0.5f);

		if (ImGui::ImageButton((ImTextureID)m_Textures[RuntimeIndex]->GetRendererID(), ImVec2{ size, size }, { 0, 1 }, { 1, 0 }))
			m_State = SceneState::Start;

		ImGui::SameLine();

		if (ImGui::ImageButton((ImTextureID)m_Textures[RuntimeDebugIndex]->GetRendererID(), ImVec2{ size, size }, { 0, 1 }, { 1, 0 }))
			m_State = SceneState::Start;

		ImGui::SameLine();

		if (ImGui::ImageButton((ImTextureID)m_Textures[StopIndex]->GetRendererID(), ImVec2{ size, size }, { 0, 1 }, { 1, 0 }))
			m_State = SceneState::Stop;

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();

		ImGui::EndChild();
	}
}