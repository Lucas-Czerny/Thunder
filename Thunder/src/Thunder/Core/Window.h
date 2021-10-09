#pragma once

#include "td_pch.h"

#include "Thunder/Core/Core.h"

#include "Thunder/Events/Event.h"

#include "Thunder/Renderer/RendererAPI.h"

namespace Thunder
{
	struct WindowProperties
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		RenderingAPI API;

		WindowProperties(const std::string& title = "My Thunder Application", uint32_t width = 1280, uint32_t height = 720, RenderingAPI renderingAPI = RenderingAPI::OpenGL)
			: Title(title), Width(width), Height(height), API(renderingAPI) {}
	};

	class THUNDER_API Window
	{
	public:
		using EventCallbackFunction = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Ref<Window> Create(const WindowProperties& properties = WindowProperties());
	};
}