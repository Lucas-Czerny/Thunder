#pragma once

#include "Thunder/Scene/SceneState.h"

#include "Thunder/Scene/Scene.h"
#include "Thunder/Scene/SceneSerializer.h"

#include "Thunder/Utils/PlatformUtils.h"

namespace fs = std::filesystem;

namespace Thunder
{
	enum class LoadSceneMode
	{
		Replace, Additive
	};

	class SceneManager
	{
	private:
		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		SceneState m_State = SceneState::Editor;

		fs::path m_CurrentSceneFilepath;

		glm::vec2 m_ViewportSize = { 1, 1 };

		std::function<void(Ref<Scene>)> m_NewSceneCallback;

	public:
		SceneManager(std::function<void(Ref<Scene>)> newSceneCallback = nullptr)
			: m_NewSceneCallback(newSceneCallback) {}

		void Update(Timestep deltaTime);

		void NewScene();
		void LoadScene(const fs::path& scenePath, const LoadSceneMode& mode = LoadSceneMode::Replace);
		void SaveScene();
		void SaveScene(const fs::path& scenePath);

		void SetViewportSize(uint32_t width, uint32_t height);

		Ref<Scene> GetActiveScene() const { return m_ActiveScene; }

		SceneState GetSceneState() const { return m_State; }
		void SetSceneState(SceneState state) { m_State = state; }
	};
}