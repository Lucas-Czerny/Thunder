#pragma once

#include "Thunder/Core/Layer.h"

#include "Thunder/Events/KeyEvent.h"
#include "Thunder/Events/MouseEvent.h"
#include "Thunder/Events/ApplicationEvent.h"

namespace Thunder
{
	class THUNDER_API ImGuiLayer : public Layer
	{
	private:
		float m_Time = 0.0f;
		bool m_BlockEvents = false;

	public:
		ImGuiLayer();
		~ImGuiLayer();

		bool* GetBlockEventsBool() { return &m_BlockEvents; }

		void OnAttach() override;
		void OnDetach() override;

		void OnEvent(Event& event) override;

		void Begin();
		void End();
	};
}

