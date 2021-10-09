#pragma once

#include <yaml-cpp/yaml.h>

#include "Thunder/Scene/Scene.h"
#include "Thunder/Scene/Entity.h"

namespace Thunder
{
	class SceneHierarchyPanel
	{
	private:
		Ref<Scene> m_Scene;
		Entity m_SelectedEntity;

	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene)
			: m_Scene(scene) {}

		void SetActiveScene(const Ref<Scene>& scene);

		void OnImGuiRender();
		void DrawSceneHierarchyPopup();

		Entity GetSelectedEntity() const { return m_SelectedEntity; }
		void SetSelectedEntity(const Entity& entity) { m_SelectedEntity = entity; }
	};
}


