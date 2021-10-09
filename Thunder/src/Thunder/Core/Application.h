#pragma once

#include "Core.h"
#include "Thunder/Core/Timestep.h"

#include "Thunder/Core/Window.h"

#include "Thunder/Events/Event.h"
#include "Thunder/Events/ApplicationEvent.h"

#include "Thunder/Core/LayerStack.h"
#include "Thunder/ImGui/ImGuiLayer.h"

namespace Thunder
{
	class THUNDER_API Application
	{
	private:
		inline static Application* s_Instance;

		Ref<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		Timestep m_LastTime = 0.0f;

	public:
		Application(const std::string& name = "My Thunder Application", uint32_t width = 1280, uint32_t height = 720, RenderingAPI renderingAPI = RenderingAPI::OpenGL);
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		void Close();

		inline static Application& Get() { return *s_Instance; }
		inline static Window& GetWindow() { return *s_Instance->m_Window; }

		void SetImGuiBlockEvents(bool block);

	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);
	};

	// to be defined in client
	Application* CreateApplication();
}