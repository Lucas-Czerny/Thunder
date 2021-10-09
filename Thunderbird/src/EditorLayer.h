#pragma once

#include <Thunder.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <ImGuizmo.h>
#include "Thunder/ImGui/CustomImGuiWidgets.h"

#include "Thunder/Projects/Project.h"
#include "Thunder/Projects/ProjectSerializer.h"

#include "SceneManager.h"

// PANELS
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/FileBrowserPanel.h"
#include "Panels/PlayPanel.h"

/*
 * script component
 * Better UI
 * duplication
 * 2d mode
 */

namespace Thunder
{
	enum OperationModeIndex
	{
		TRANSLATE = 0, SCALE = 4, ROTATE = 8,
		ALL = 0, X = 1, Y = 2, Z = 3
	};

	class EditorLayer : public Layer
	{
	private:
		Ref<Framebuffer> m_Framebuffer;

		glm::vec2 m_ViewportPosition;
		glm::vec2 m_ViewportSize;
		glm::vec2 m_ViewportMousePosition;

		Project m_CurrentProject;

		SceneManager m_SceneManager;

		SceneHierarchyPanel m_SceneHierarchyPanel;
		PropertiesPanel m_PropertiesPanel;
		FileBrowserPanel m_FileBrowserPanel;
		PlayPanel m_PlayPanel;

		Entity m_SquareEntity;
		Entity m_CameraEntity;

		float m_Size = 1.0f;

		int m_OperationModeIndex = OperationModeIndex::TRANSLATE;
		int m_OperationModeAxis = OperationModeIndex::ALL;

		static const std::array<ImGuizmo::OPERATION, 12> m_OperationModes;

		bool m_AllowMousePicking = false;

	public:
		EditorLayer()
			: Layer("EditorLayer"), m_FileBrowserPanel(m_SceneManager, "") {}

		~EditorLayer() {}

		void OnAttach() override;
		void OnUpdate(Timestep deltaTime) override;
		void OnImGuiRender() override;
		void OnEvent(Event& event) override;

	private:
		void NewProjectDialog();
		void OpenProject(const Project& project);
		void OpenProjectDialog();

		void SaveProject();

		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& event);
		bool OnKeyPressed(KeyPressedEvent& event);
	};
}
