#pragma once

#include "Thunder/Core/Core.h"
#include "Thunder/Core/Timestep.h"

#include "Thunder/Events/Event.h"

namespace Thunder
{
	class THUNDER_API Layer
	{
	protected:
		std::string m_Name;

	public:
		Layer(const std::string& debugName);

		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(Timestep deltaTime) {}
		virtual void OnEvent(Event& event) {}

		virtual void OnImGuiRender() {}

		inline const std::string& GetName() const { return m_Name; }
	};
}

