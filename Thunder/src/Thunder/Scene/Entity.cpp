#include "td_pch.h"
#include "Entity.h"

namespace Thunder
{
	void Entity::Destroy()
	{
		m_Scene->m_Registry.destroy(m_EntityHandle);
		m_EntityHandle = s_NullEntity;
	}
}