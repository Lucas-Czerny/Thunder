#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Thunder/Renderer/GraphicsContext.h"

namespace Thunder
{
	class OpenGLContext : public GraphicsContext
	{
	private:
		GLFWwindow* m_WindowHandle;

	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() override;
	};
}