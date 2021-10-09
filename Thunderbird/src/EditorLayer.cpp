#include "td_pch.h"
#include "EditorLayer.h"

#include <EnTT/entt.hpp>

#include "Thunder/Core/Config.h"
#include "Thunder/ImGui/ImGuiConfig.h"

#include "Thunder/Math/MatrixDecompose.h"

namespace Thunder
{
	const std::array<ImGuizmo::OPERATION, 12> EditorLayer::m_OperationModes = {
		ImGuizmo::OPERATION::TRANSLATE,
		ImGuizmo::OPERATION::TRANSLATE_X,
		ImGuizmo::OPERATION::TRANSLATE_Y,
		ImGuizmo::OPERATION::TRANSLATE_Z,

		ImGuizmo::OPERATION::SCALE,
		ImGuizmo::OPERATION::SCALE_X,
		ImGuizmo::OPERATION::SCALE_Y,
		ImGuizmo::OPERATION::SCALE_Z,

		ImGuizmo::OPERATION::ROTATE,
		ImGuizmo::OPERATION::ROTATE_X,
		ImGuizmo::OPERATION::ROTATE_Y,
		ImGuizmo::OPERATION::ROTATE_Z,
	};

	void EditorLayer::OnAttach()
	{
		FramebufferSpecification specification;
		specification.Width = 1280;
		specification.Height = 720;

		specification.Attachments = { FramebufferFormat::RGBA8, FramebufferFormat::DEPTH, FramebufferFormat::R32I };

		m_Framebuffer = Framebuffer::Create(specification);

		m_SceneManager = SceneManager([&](Ref<Scene> newScene) {
			m_SceneHierarchyPanel.SetActiveScene(newScene);
		});
		m_PropertiesPanel.SetContext(&m_SceneHierarchyPanel);

		m_SceneManager.NewScene();

		Renderer::SetClearColor(Config::ClearColor);
	}

	void EditorLayer::OnUpdate(Timestep deltaTime) 
	{
		m_Framebuffer->Bind();

		Renderer::ClearColor();

		m_Framebuffer->ClearAttachment(1, -1);

		m_SceneManager.Update(deltaTime);

		if (m_AllowMousePicking)
		{
			int32_t entityID = m_Framebuffer->ReadPixel(m_ViewportMousePosition.x, m_ViewportMousePosition.y, 1);

			if (entityID != -1)
			{
				Ref<Scene> activeScene = m_SceneManager.GetActiveScene();
				Entity entity = Entity((entt::entity)entityID, activeScene.get());
				m_SceneHierarchyPanel.SetSelectedEntity(entity);
			}
			else
			{
				m_SceneHierarchyPanel.SetSelectedEntity({});
			}

			m_AllowMousePicking = false;
		}

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		Ref<Scene> activeScene = m_SceneManager.GetActiveScene();

		// ---------
		// DOCKSPACE
		// ---------

		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGuiStyle& style = ImGui::GetStyle();

		float defaultWindowMinSizeX = style.WindowMinSize.x;
		float defaultWindowMinSizeY = style.WindowMinSize.x;
		style.WindowMinSize.x = Config::DockspaceWindowMinSize.x;
		style.WindowMinSize.y = Config::DockspaceWindowMinSize.y;

		static bool open = true;
		static bool shouldClose = false;
		if (!ImGui::Begin("DockSpace Demo", &open, window_flags))
		{
			ImGui::End();
			return;
		}

		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		static bool settingsWindowOpen = false;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Project", "Ctrl + N"))
				{
					NewProjectDialog();
				}

				if (ImGui::MenuItem("Open Project", "Ctrl + O"))
				{
					OpenProjectDialog();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Save Scene", "Ctrl + S"))
				{
					m_SceneManager.SaveScene();
				}

				if (ImGui::MenuItem("Save Scene As", "Ctrl + S"))
				{
					fs::path newScenePath = FileDialog::OpenFileDialog(FILE_DIALOG_FILTER("Thunder Scene File", ".td"));
					m_SceneManager.SaveScene(newScenePath);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Alt + F4"))
				{
					shouldClose = true;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings"))
			{
				if (ImGui::MenuItem("Settings"))
				{
					settingsWindowOpen = true;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();

		style.WindowMinSize.x = defaultWindowMinSizeX;
		style.WindowMinSize.y = defaultWindowMinSizeY;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

		// -----------------
		// Panels
		// -----------------

		m_SceneHierarchyPanel.OnImGuiRender();
		m_PropertiesPanel.OnImGuiRender();
		m_FileBrowserPanel.OnImGuiRender();

		ImGui::Begin("PlayPanel + Viewport");

		// No tab bar
		if (ImGui::IsWindowDocked())
		{
			auto* window = ImGui::FindWindowByName("PlayPanel + Viewport");
			if (window)
			{
				ImGuiDockNode* node = window->DockNode;
				node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
			}
		}

		m_PlayPanel.OnImGuiRender();
		SceneState state = m_PlayPanel.GetState();
		m_SceneManager.SetSceneState(state);

		// -----------------
		// Game Viewport
		// -----------------

		ImGui::BeginChild("Viewport");

		Application::Get().SetImGuiBlockEvents(!ImGui::IsWindowFocused() & !ImGui::IsWindowHovered());

		ImVec2 pos = ImGui::GetWindowPos();
		m_ViewportPosition = { pos.x, pos.y };

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();

		if (m_ViewportSize != (glm::vec2&)viewportSize)
		{
			m_ViewportSize = { viewportSize.x, viewportSize.y };
			m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
			m_SceneManager.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, viewportSize, { 0, 1 }, { 1, 0 });

		ImGui::PopStyleVar();

		// -----------------
		// Gizmos
		// -----------------

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

		if (Input::Shortcut(TD_KEY_LEFT_CONTROL, TD_KEY_D))
		{
			activeScene->DuplicateEntity(selectedEntity);
		}

		if (selectedEntity && m_SceneManager.GetSceneState() == SceneState::Editor)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float x = m_ViewportPosition.x, y = m_ViewportPosition.y;
			float width = m_ViewportSize.x, height = m_ViewportSize.y;

			ImGuizmo::SetRect(x, y, width, height);

			EditorCamera& editorCamera = activeScene->GetEditorCamera();

			auto& entityTransformComponent = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 entityTransform = entityTransformComponent.GetTransform();

			ImGuizmo::OPERATION operation = m_OperationModes[m_OperationModeIndex + m_OperationModeAxis];

			ImGuizmo::Manipulate(glm::value_ptr(editorCamera.GetViewMatrix()), glm::value_ptr(editorCamera.GetProjectionMatrix()),
				operation, ImGuizmo::LOCAL,
				glm::value_ptr(entityTransform));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 scale;
				glm::vec3 rotation;
				glm::vec3 translation;
				Math::DecomposeTransform(entityTransform, translation, rotation, scale);

				entityTransformComponent.Translation = translation;
				entityTransformComponent.Scale = scale;
				entityTransformComponent.Rotation = rotation;
			}
		}

		ImGui::EndChild();
		ImGui::End();

		// -----------------
		// Settings
		// -----------------

		if (settingsWindowOpen)
		{
			ImGui::Begin("Settings");

			ImGui::Text("Settings");
			ImGui::Spacing();

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

			if (ImGui::TreeNodeEx("Advanced"))
			{
				ImCustom::SliderInt("VelocityIterations", &Config::VelocityIterations, Config::PhysicsIterationSlider);
				ImCustom::SliderInt("PositionIterations", &Config::PositionIterations, Config::PhysicsIterationSlider);

				ImGui::TreePop();
			}

			ImGui::End();
		}

		// -----------------
		// Dialogs
		// -----------------

		ImCustom::DrawDialog("New Project", [&]()
		{
			static Project newProject;

			ImGui::Text("Create a new project");

			static char nameBuffer[256];
			memset(nameBuffer, '\0', sizeof(nameBuffer));
			strcpy_s(nameBuffer, sizeof(nameBuffer), newProject.Name.c_str());

			ImGui::InputText("Project Name", nameBuffer, 256);
			newProject.Name = nameBuffer;

			if (ImGui::Button("Select Folder"))
			{
				fs::path directory = FileDialog::OpenFolderDialog();
				newProject.ProjectDirectory = directory.string();
			}

			ImGui::Spacing();

			if (ImGui::Button("OK"))
				ImCustom::CloseDialog(DialogResponse::OK, newProject);

			ImGui::SameLine();

			if (ImGui::Button("Cancel"))
				ImCustom::CloseDialog(DialogResponse::CANCEL, newProject);
		});
	}

	void EditorLayer::NewProjectDialog()
	{
		ImCustom::OpenDialog<Project>("New Project", [&](DialogResponse response, Project newProject)
		{
			if (response == DialogResponse::CANCEL)
				return;

			if (newProject.Name.empty() || newProject.ProjectDirectory.empty())
			{
				TD_CORE_ERROR("Invalid Input Bro............");
				return;
			}

			OpenProject(newProject);
				
			fs::path projectFilePath = m_CurrentProject.GetProjectFilePath();
			fs::path assetsFilePath = m_CurrentProject.GetAssetsPath();

			if (fs::exists(projectFilePath))
				TD_CORE_ERROR("Project already exists. I am overwriting it MUHAHAHA");

			fs::create_directory(assetsFilePath);

			ProjectSerializer serializer(m_CurrentProject);
			serializer.SerializeYAML(projectFilePath);
		});
	}

	void EditorLayer::OpenProject(const Project& project)
	{
		TD_CORE_INFO("Opening project {0}", m_CurrentProject.Name);
		TD_CORE_INFO(" -> Project Directory {0}", m_CurrentProject.ProjectDirectory);
		TD_CORE_INFO(" -> StartUpScenePath {0}", m_CurrentProject.StartupScenePath);

		m_CurrentProject = project;
		m_FileBrowserPanel.SetContext(m_SceneManager, (fs::path)project.ProjectDirectory / "assets");

		m_SceneManager.LoadScene(m_CurrentProject.StartupScenePath);
	}

	void EditorLayer::OpenProjectDialog()
	{
		fs::path directory = FileDialog::OpenFolderDialog();
		
		Project project;
		ProjectSerializer serializer(project);
		serializer.DeserializeYAML(directory / ".thunder");

		OpenProject(project);
	}

	void EditorLayer::SaveProject()
	{

	}

	void EditorLayer::OnEvent(Event& event)
	{
		Ref<Scene> activeScene = m_SceneManager.GetActiveScene();
		activeScene->OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(TD_BIND_EVENT_FUNCTION(EditorLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(TD_BIND_EVENT_FUNCTION(EditorLayer::OnMouseButtonReleased));
		dispatcher.Dispatch<KeyPressedEvent>(TD_BIND_EVENT_FUNCTION(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		ImVec2 mousePosition = ImGui::GetMousePos();
		m_ViewportMousePosition = { mousePosition.x - m_ViewportPosition.x, m_ViewportSize.y - mousePosition.y + m_ViewportPosition.y };

		if (event.GetMouseButton() == TD_MOUSE_BUTTON_LEFT && !ImGuizmo::IsOver() &&
			// Mouse is inside of the viewport
			m_ViewportMousePosition.x > 0.0f && m_ViewportMousePosition.y > 0.0f &&
			m_ViewportMousePosition.x < m_ViewportSize.x && m_ViewportMousePosition.y < m_ViewportSize.y)
		{
			m_AllowMousePicking = true;
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& event)
	{
		if (event.GetMouseButton() == TD_MOUSE_BUTTON_LEFT)
			m_AllowMousePicking = false;

		return false;
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		switch (event.GetKeyCode())
		{
			case TD_KEY_Q:
			{
				m_OperationModeIndex = OperationModeIndex::TRANSLATE;
				break;
			}
			case TD_KEY_W:
			{
				m_OperationModeIndex = OperationModeIndex::SCALE;
				break;
			}
			case TD_KEY_E:
			{
				m_OperationModeIndex = OperationModeIndex::ROTATE;
				break;
			}
			case TD_KEY_X:
			case TD_KEY_Y:
			case TD_KEY_Z:
			{
				m_OperationModeAxis = OperationModeIndex::Z - TD_KEY_Z + event.GetKeyCode();
				break;
			}
			default:
			{
				m_OperationModeAxis = OperationModeIndex::ALL;
			}
		}

		// SHORTCUTS

		if (event.GetRepeatCount() > 0)
			return false;

		bool control_modifier = Input::GetKeyDown(TD_KEY_LEFT_CONTROL) || Input::GetKeyDown(TD_KEY_RIGHT_CONTROL);
		bool shift_modifier = Input::GetKeyDown(TD_KEY_LEFT_SHIFT) || Input::GetKeyDown(TD_KEY_RIGHT_SHIFT);

		switch (event.GetKeyCode())
		{
			case TD_KEY_N:
			{
				if (control_modifier)
					NewProjectDialog();

				break;
			}
			case TD_KEY_O:
			{
				if (control_modifier)
					OpenProjectDialog();

				break;
			}
			case TD_KEY_S:
			{
				if (control_modifier)
					m_SceneManager.SaveScene();

				if (control_modifier && shift_modifier)
				{
					fs::path newScenePath = FileDialog::OpenFileDialog(FILE_DIALOG_FILTER("Thunder Scene File", ".td"));
					m_SceneManager.SaveScene(newScenePath);
				}

				break;
			}
		}
	}
}