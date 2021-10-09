#pragma once

#include "Thunder/Renderer/Texture.h"

#include "Thunder/Scene/Scene.h"
#include "../SceneManager.h"

namespace fs = std::filesystem;

namespace Thunder
{
	class FileBrowserPanel
	{
		friend struct File;

	private:
		enum Icons
		{
			ArrowUp = 0, ArrowLeft, ArrowRight,
			Folder, Unknown, Scene
		};

		SceneManager& m_SceneManager;

		fs::path m_AssetsPath;
		fs::path m_CurrentDirectory;

		std::array<Ref<Texture>, 6> m_Textures;

		uint32_t m_ClickCounter = 0;

	public:
		FileBrowserPanel(SceneManager& sceneManager, const fs::path& assetsPath);

		void SetContext(SceneManager& sceneManager, const fs::path& assetsPath);

		void OnImGuiRender();

	private:
		void DrawDirectoryNode(const fs::path& directory);
		void DrawDirectoryContent(const fs::path& directory);
	};

	enum class FileType
	{
		UNKNOWN = 0,
		DIRECTORY, MUSIC,
		SCENE, SHADER
	};

	typedef void (*FileClickedCallback)();

	struct File
	{
	private:
		FileBrowserPanel& m_PanelContext;

	public:
		FileType Type;
		fs::path Path;

		std::function<void()> ClickCallback;

		Ref<Texture> Icon;

		File(FileBrowserPanel& panel, const FileType& type, const fs::path& path);
	};
}

