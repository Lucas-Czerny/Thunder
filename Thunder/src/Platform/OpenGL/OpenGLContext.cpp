#include "td_pch.h"

#include "OpenGLContext.h"

namespace Thunder
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle) {}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		TD_CORE_ASSERT(status, "Failed to initialize GLAD!");

		TD_CORE_INFO("OpenGL Renderer:");
		TD_CORE_INFO("-> Vendor: {0}", glGetString(GL_VENDOR));
		TD_CORE_INFO("-> Renderer: {0}", glGetString(GL_RENDERER));
		TD_CORE_INFO("-> Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}