#pragma once

#include "Thunder/Core/Window.h"
#include "Thunder/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Thunder
{
	class WindowsWindow : public Window
	{
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync;
			RenderingAPI API;

			EventCallbackFunction EventCallback;
		} m_Data;
		
	public:
		WindowsWindow(const WindowProperties& properties);
		~WindowsWindow();

		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFunction& callback) override { m_Data.EventCallback = callback; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline void* GetNativeWindow() const { return m_Window; }

	private:
		void Init(const WindowProperties& properties);
		void Shutdown();
	};
}