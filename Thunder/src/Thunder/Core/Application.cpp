#include "td_pch.h"
#include "Application.h"

#include "glm/glm.hpp"

#include "Thunder/Core/Log.h"
#include "Thunder/Core/Input.h"

#include "Thunder/Renderer/Renderer.h"

#include "Thunder/Core/Window.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Thunder
{
	Application::Application(const std::string& name /* = "My Thunder Application" */, uint32_t width /* = 1280 */, uint32_t height /* = 720 */, RenderingAPI renderingAPI /* = RenderingAPI::OpenGL */)
	{
		TD_PROFILE_FUNCTION();

		TD_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create({ name, width, height, renderingAPI });
		m_Window->SetEventCallback(TD_BIND_EVENT_FUNCTION(Application::OnEvent));
		m_Window->SetVSync(true);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		TD_PROFILE_FUNCTION();
	}

	void Application::Run()
	{
		TD_PROFILE_FUNCTION();

		while (m_Running)
		{
			Timestep time = glfwGetTime();
			Timestep deltaTime = time - m_LastTime;
			m_LastTime = time;

			if (!m_Minimized)
			{
				{
					TD_PROFILE_SCOPE("Updating all Layers");

					for (Layer* layer : m_LayerStack)
					{
						layer->OnUpdate(deltaTime);
					}
				}
			}

			{
				TD_PROFILE_SCOPE("ImGui Render for all Layers");

				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		TD_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(TD_BIND_EVENT_FUNCTION(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(TD_BIND_EVENT_FUNCTION(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PopOverlay(Layer* overlay)
	{
		m_LayerStack.PopOverlay(overlay);
		overlay->OnDetach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		Close();
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		TD_PROFILE_FUNCTION();

		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(event);
		
		return false;
	}

	void Application::SetImGuiBlockEvents(bool block)
	{
		bool* blockEventsBool = m_ImGuiLayer->GetBlockEventsBool();
		*blockEventsBool = block;
	}
};