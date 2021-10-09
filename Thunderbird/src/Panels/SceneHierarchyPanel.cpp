#include "td_pch.h"
#include "SceneHierarchyPanel.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <imgui.h>

#include "Thunder/Core/MouseButtonCodes.h"

#include "Thunder/Scene/Components.h"

namespace Thunder
{
	void SceneHierarchyPanel::SetActiveScene(const Ref<Scene>& scene)
	{
		m_Scene = scene;
		m_SelectedEntity = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		ImGui::Spacing();

		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = Entity(entityID, m_Scene.get());
			auto& tagComponent = entity.GetComponent<TagComponent>();

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

			if (m_SelectedEntity == entity)
				flags |= ImGuiTreeNodeFlags_Selected;

			bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entityID, flags, tagComponent.Tag.c_str());

			if (ImGui::IsItemClicked())
			{
				m_SelectedEntity = entity;
			}

			if (ImGui::BeginPopupContextItem())
			{
				ImGui::Spacing();

				if (ImGui::MenuItem(" Delete Entity"))
				{
					entity.Destroy();
					m_SelectedEntity = {};
				}

				ImGui::Spacing();

				ImGui::EndPopup();
			}

			if (open)
			{
				ImGui::Text("Sub Entities");

				ImGui::TreePop();
			}
		});


		if (ImGui::IsMouseDown(TD_MOUSE_BUTTON_LEFT) && ImGui::IsWindowHovered())
		{
			m_SelectedEntity = {};
		}

		if (ImGui::BeginPopupContextWindow(0, TD_MOUSE_BUTTON_RIGHT, false))
		{
			DrawSceneHierarchyPopup();

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawSceneHierarchyPopup()
	{
		ImGui::Spacing();

		if (ImGui::MenuItem(" Add Entity"))
		{
			m_Scene->CreateEntity();
		}

		ImGui::Spacing();
	}
}