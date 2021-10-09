#include "td_pch.h"
#include "SceneManager.h"

namespace Thunder
{
	void SceneManager::Update(Timestep deltaTime)
	{
		switch (m_State)
		{
		case SceneState::Start:
			m_ActiveScene->OnRuntimeStart();
			m_EditorScene = m_ActiveScene->Copy();

		case SceneState::Runtime:
			m_ActiveScene->OnRuntimeUpdate(deltaTime);
			break;

		case SceneState::Stop:
			m_ActiveScene->OnRuntimeStop();
			m_ActiveScene = m_EditorScene->Copy();
			m_ActiveScene->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

		case SceneState::Editor:
			m_ActiveScene->OnEditorUpdate(deltaTime);
			break;
		}
	}

	void SceneManager::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_ViewportSize = { width, height };
		m_ActiveScene->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
	}

	void SceneManager::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();

		m_ActiveScene->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

		if (m_NewSceneCallback)
			m_NewSceneCallback(m_ActiveScene);

		m_EditorScene = m_ActiveScene;
	}

	void SceneManager::LoadScene(const fs::path& scenePath, const LoadSceneMode& mode /* = LoadSceneMode::Replace */)
	{
		m_CurrentSceneFilepath = scenePath;

		if (mode != LoadSceneMode::Additive)
			NewScene();

		if (scenePath.empty())
			return;

		SceneSerializer serializer(m_ActiveScene);
		serializer.DeserializeYAML(scenePath);

		m_EditorScene = m_ActiveScene;
	}

	void SceneManager::SaveScene()
	{
		TD_CORE_INFO("Saved Scene!");

		if (!m_CurrentSceneFilepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.SerializeYAML(m_CurrentSceneFilepath);

			m_EditorScene = m_ActiveScene;
		}
	}

	void SceneManager::SaveScene(const fs::path& scenePath)
	{
		TD_CORE_INFO("Saved Scene!");

		if (!scenePath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.SerializeYAML(scenePath);

			m_EditorScene = m_ActiveScene;
		}
	}
}