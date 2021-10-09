#pragma once

#include <entt/entt.hpp>

#include "Thunder/Core/Timestep.h"
#include "Thunder/Core/GUID.h"

#include "Thunder/Events/Event.h"

#include "Thunder/Scene/EditorCamera.h"

class b2World;

namespace Thunder
{
	class Entity;

	class Scene
	{
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

	private:
		entt::registry m_Registry;
		b2World* m_PhysicsWorld = nullptr;

		float m_ViewportWidth = 1.0f, m_ViewportHeight = 1.0f;

		EditorCamera m_EditorCamera;

	public:
		Scene();
		~Scene();

		Ref<Scene> Copy();

		void OnEditorUpdate(Timestep deltaTime);
		void OnRuntimeUpdate(Timestep deltaTime);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnEvent(Event& event);

		void SetViewportSize(uint32_t width, uint32_t height);

		Entity CreateEntity(const std::string& name = "Emptity", const GUID& guid = GUID::Generate());
		void DuplicateEntity(Entity& entity);

		template <typename T>
		void OnComponentAdded(const Entity& entity, T& component);

		EditorCamera& GetEditorCamera() { return m_EditorCamera; }

	private:
		template <typename T>
		static void CopyComponentHelper(entt::registry& srcRegistry, entt::registry& destRegistry, std::unordered_map<GUID, entt::entity> idMap);

		template <typename T>
		static void DuplicateEntityHelper(Entity& from, Entity& to);
	};
}