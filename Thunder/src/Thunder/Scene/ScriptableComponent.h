#pragma once

#include "Thunder/Core/Timestep.h"
#include "Thunder/Events/Event.h"

#include "Entity.h"

namespace Thunder
{
	class Entity;

	class ScriptableComponent
	{
		friend class Scene;

	private:
		Entity m_Entity;
			
	public:
		virtual ~ScriptableComponent() {}

	protected:
		virtual void OnCreate() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnDestroy() {}
		virtual void OnEvent(Event& event) {}

		template <typename T>
		bool HasComponent()
		{
			return m_Entity.HasComponent<T>();
		}

		template <typename T, typename... Args>
		T& AddComponent(const Args&... args)
		{
			return m_Entity.AddComponent<T>();
		}

		template <typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		template <typename T>
		void RemoveComponent()
		{
			return m_Entity.RemoveComponent<T>();
		}
	};
}