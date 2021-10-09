#include "td_pch.h"

namespace Thunder
{
	template <typename T>
	bool Entity::HasComponent()
	{
		return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
	}

	template <typename T, typename... Args>
	T& Entity::AddComponent(const Args&... args)
	{
		TD_CORE_ASSERT(!HasComponent<T>(), "Entity already has the specified component!");

		T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, args...);
		m_Scene->OnComponentAdded<T>(*this, component);

		return component;
	}

	template <typename T, typename... Args>
	T& Entity::AddOrReplaceComponent(const Args&... args)
	{
		T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, args...);
		m_Scene->OnComponentAdded<T>(*this, component);

		return component;
	}

	template <typename T>
	T& Entity::GetComponent()
	{
		TD_CORE_ASSERT(HasComponent<T>(), "Entity does not have the specified component!");

		return m_Scene->m_Registry.get<T>(m_EntityHandle);
	}

	template <typename T>
	void Entity::RemoveComponent()
	{
		TD_CORE_ASSERT(HasComponent<T>());

		m_Scene->m_Registry.remove<T>(m_EntityHandle);
	}
}