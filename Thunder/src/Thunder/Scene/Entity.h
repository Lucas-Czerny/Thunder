#pragma once

#include <entt/entt.hpp>

#include "Scene.h"

namespace Thunder
{
	class Entity
	{
		friend class Scene;

	private:
		static const entt::entity s_NullEntity = entt::null;
		entt::entity m_EntityHandle = s_NullEntity;
		Scene* m_Scene = nullptr;

		std::string m_Name;

	public:
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(entt::entity entity, Scene* scene) 
			: m_EntityHandle(entity), m_Scene(scene) {}

		void Destroy();

		template <typename T>
		bool HasComponent();

		template <typename T, typename... Args>
		T& AddComponent(const Args&... args);

		template <typename T, typename... Args>
		T& AddOrReplaceComponent(const Args&... args);

		template <typename T>
		T& GetComponent();

		template <typename T>
		void RemoveComponent();

		std::string GetName() const { return m_Name; }

		operator bool () { return m_EntityHandle != s_NullEntity; }
		operator entt::entity () const { return m_EntityHandle; }
		operator int32_t () const { return (int32_t)m_EntityHandle; }

		bool operator==(const Entity& other)
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}
		
		bool operator!=(const Entity& other)
		{
			return !(*this == other);
		}
	};
}

#include "Entity.inl"