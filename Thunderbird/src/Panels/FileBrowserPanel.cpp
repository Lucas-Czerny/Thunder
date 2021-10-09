#include "td_pch.h"
#include "FileBrowserPanel.h"

#include <imgui.h>

#include "Thunder/ImGui/ImGuiConfig.h"

#include "Thunder/ImGui/CustomImGuiWidgets.h"

#include "Thunder/Core/Input.h"
#include "Thunder/Core/KeyCodes.h"
#include "Thunder/Core/MouseButtonCodes.h"

namespace Thunder
{
	FileBrowserPanel::FileBrowserPanel(SceneManager& sceneManager, const fs::path& assetsPath)
		: m_SceneManager(sceneManager), m_AssetsPath(assetsPath), m_CurrentDirectory(m_AssetsPath)
	{
		m_Textures[ArrowUp] = Texture2D::Create("resources/icons/arrow_up.png");
		m_Textures[ArrowLeft] = Texture2D::Create("resources/icons/arrow_left.png");
		m_Textures[ArrowRight] = Texture2D::Create("resources/icons/arrow_right.png");

		m_Textures[Folder] = Texture2D::Create("resources/icons/folder.png");
		m_Textures[Unknown] = Texture2D::Create("resources/icons/unknown.png");
		m_Textures[Scene] = Texture2D::Create("resources/icons/scene.png");
	}

	void FileBrowserPanel::SetContext(SceneManager& sceneManager, const fs::path& assetsPath)
	{
		m_SceneManager = sceneManager;
		m_AssetsPath = assetsPath;
		m_CurrentDirectory = m_AssetsPath;
	}

	void FileBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("File Browser");

		int windowWidth = ImGui::GetContentRegionAvail().x;
		ImVec2 windowPadding = (ImVec2&)Config::FileBrowserPadding;

		ImGui::Columns(2, "File Browser Columns");

		static float initialColumnWidth = windowWidth / 4;
		if (initialColumnWidth)
			ImGui::SetColumnWidth(0, initialColumnWidth);
		initialColumnWidth = 0.0f;

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + windowPadding.y);

		ImGui::BeginChild("Overview");

		DrawDirectoryNode(m_AssetsPath);

		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + windowPadding.x);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + windowPadding.y);

		if (ImGui::ImageButton((ImTextureID)m_Textures[ArrowLeft]->GetRendererID(), ImVec2{ 15.5, 17 }))
		{
		}

		ImGui::SameLine();

		if (ImGui::ImageButton((ImTextureID)m_Textures[ArrowRight]->GetRendererID(), ImVec2{ 15.5, 17 }))
		{
		}

		ImGui::SameLine();

		if (ImGui::ImageButton((ImTextureID)m_Textures[ArrowUp]->GetRendererID(), ImVec2{ 15.5, 17 }, { 0, 1 }, { 1, 0 }) &&
			m_CurrentDirectory.has_parent_path())
		{
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
		}

		ImGui::SameLine();

		std::string directory_str = m_CurrentDirectory.string();
		ImGui::InputText("##directory", (char*)directory_str.c_str(), directory_str.size(), ImGuiInputTextFlags_ReadOnly);

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + windowPadding.x);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + windowPadding.y);

		ImGui::BeginChild("Browser");

		DrawDirectoryContent(m_CurrentDirectory);

		ImGui::EndChild();

		ImGui::Columns(1);

		ImGui::End();
	}

	void FileBrowserPanel::DrawDirectoryNode(const fs::path& directory)
	{
		if (directory.empty())
			return;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		std::string filename = directory.filename().string();
		bool open = ImGui::TreeNodeEx(filename.c_str(), flags);

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(TD_MOUSE_BUTTON_LEFT))
		{
			m_CurrentDirectory = directory;
		}

		if (open)
		{
			for (const auto& entry : fs::directory_iterator(directory))
			{
				const fs::path path = entry.path();

				if (entry.is_directory())
				{
					DrawDirectoryNode(path);
				}
			}

			ImGui::TreePop();
		}
	}

	void FileBrowserPanel::DrawDirectoryContent(const fs::path& directory)
	{
		if (directory.empty())
			return;

		ImGuiStyle& style = ImGui::GetStyle();
		float rightBorder = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		bool rightClicked = ImGui::IsMouseClicked(TD_MOUSE_BUTTON_RIGHT) && ImGui::IsWindowHovered();
		bool hovered = false;

		for (const auto& entry : fs::directory_iterator(directory))
		{
			const fs::path path = entry.path();
			const fs::path extension = path.extension();

			FileType type = FileType::UNKNOWN;

			if (entry.is_directory())
				type = FileType::DIRECTORY;
			else if (extension == ".td")
				type = FileType::SCENE;

			File file(*this, type, path);

			ImGui::SetNextItemWidth(100);
			ImGui::BeginGroup();

			float size = Config::FileIconSize;

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)file.Icon->GetRendererID(), ImVec2{ size, size }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginDragDropSource())
				{
					File* filePayload = new File(file);
					ImGui::SetDragDropPayload("FILE", &filePayload, sizeof(File), ImGuiCond_Once);
					ImGui::EndDragDropSource();
				}

				if (ImGui::IsMouseDoubleClicked(TD_MOUSE_BUTTON_LEFT))
					file.ClickCallback();

				hovered = true;
			}

			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + size);

			std::string filename = path.filename().string();

			ImCustom::CenterAlignedText(filename.c_str(), size);

			ImGui::PopTextWrapPos();

			ImGui::EndGroup();

			float lastButtonX = ImGui::GetItemRectMax().x;
			float nextButtonX = lastButtonX + style.ItemSpacing.x + size;

			if (nextButtonX < rightBorder)
				ImGui::SameLine();
		}

		ImCustom::DrawPopup("Create Popup", rightClicked && !hovered, [&]()
		{
			bool selected = true;

			ImGui::PushItemWidth(300.0f);

			if (ImGui::MenuItem("Create Folder"))
			{
				fs::create_directory(m_CurrentDirectory / "New Folder");
			}
			else if (ImGui::MenuItem("Create Empty Scene"))
			{
				std::ofstream ofs(m_CurrentDirectory / "New Scene.td");
				ofs.close();
			}
			else
			{
				selected = false;
			}

			return selected;
		});

		static bool openRenameDialog = false;

		ImCustom::DrawPopup("Action Popup", rightClicked && hovered, [&]()
		{
			bool selected = true;

			ImGui::PushItemWidth(300.0f);

			if (ImGui::MenuItem("Rename"))
			{
				ImCustom::OpenDialog<const char*>("Rename Dialog", [&](DialogResponse response, const char* buffer)
				{
					if (response == DialogResponse::CANCEL)
						return;

					TD_CORE_INFO("Rename to {0}", buffer);
				});
			}
			else
			{
				selected = false;
			}

			return selected;
		});

		ImCustom::DrawDialog("Rename Dialog", [&]()
		{
			TD_CORE_ERROR("Hello??");
			static char nameBuffer[MAX_PATH];

			if (nameBuffer[0] != '\0')
				memset(nameBuffer, '\0', MAX_PATH);

			ImGui::InputText("New Name", nameBuffer, MAX_PATH);

			if (ImGui::Button("OK"))
				ImCustom::CloseDialog(DialogResponse::OK, nameBuffer);

			ImGui::SameLine();

			if (ImGui::Button("Cancel"))
				ImCustom::CloseDialog(DialogResponse::CANCEL, nameBuffer);
		});
	}

	File::File(FileBrowserPanel& panel, const FileType& type, const fs::path& path)
		: m_PanelContext(panel), Type(type), Path(path)
	{
		switch (Type)
		{
		case FileType::DIRECTORY:
			ClickCallback = [&]()
			{
				m_PanelContext.m_CurrentDirectory = Path;
			};
			Icon = m_PanelContext.m_Textures[m_PanelContext.Folder];

			break;

		case FileType::SCENE:
			ClickCallback = [&]()
			{
				m_PanelContext.m_SceneManager.LoadScene(Path.string());
			};
			Icon = m_PanelContext.m_Textures[m_PanelContext.Scene];

			break;

		default:
			ClickCallback = [&]() {};
			Icon = m_PanelContext.m_Textures[m_PanelContext.Unknown];

			break;
		}
	}
}