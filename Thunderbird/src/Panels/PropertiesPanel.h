#pragma once

#include "SceneHierarchyPanel.h"

namespace Thunder
{
	class PropertiesPanel
	{
	private:
		SceneHierarchyPanel* m_Context;
		Entity m_SelectedEntity;

	public:
		PropertiesPanel() = default;
		PropertiesPanel(SceneHierarchyPanel* context)
			: m_Context(context) {}

		void SetContext(SceneHierarchyPanel* context) { m_Context = context; }

		void OnImGuiRender();

		template <typename T>
		void DrawComponent(std::function<void(T&)> componentUICode, bool removeable = true);
	};
}


