#include "td_pch.h"

#include <fstream>

#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Thunder
{
	Ref<Shader> Shader::Create(const std::string& shaderPath)
	{
		int lastSlash = shaderPath.find_last_of("/\\");
		int firstLetter = lastSlash == std::string::npos ? 0 : lastSlash + 1;

		int fileExtension = shaderPath.rfind(".glsl");
		int count = fileExtension == std::string::npos ? shaderPath.size() - firstLetter : fileExtension - firstLetter;

		std::string name = shaderPath.substr(firstLetter, count);

		ShaderSource shaderSource = GetShaderSourceFromFile(shaderPath);

		return CreateImpl(name, shaderSource);
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
	{
		ShaderSource shaderSource;
		shaderSource.vertexShaderSource = vertexShaderSource;
		shaderSource.fragmentShaderSource = fragmentShaderSource;

		return CreateImpl(name, shaderSource);
	}

	Ref<Shader> Shader::CreateImpl(const std::string& name, ShaderSource shaderSource)
	{
		switch (Renderer::GetRenderingAPI())
		{
			case RenderingAPI::None:
			{
				TD_CORE_ASSERT(false, "RendererAPI is not set (RendererAPI::None)");
				return nullptr;
			}
			case RenderingAPI::OpenGL:
			{
				return CreateRef<OpenGLShader>(name, shaderSource);
			}
		}

		TD_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	ShaderSource Shader::GetShaderSourceFromFile(const std::string& shaderPath)
	{
		std::ifstream inputFile(shaderPath);

		ShaderSource source;
		std::string currentSource;

		if (inputFile.is_open())
		{
			std::string line;
			while (std::getline(inputFile, line))
			{
				if (line == "#type vertex")
				{
					continue;
				}
				else if (line == "#type fragment")
				{
					source.vertexShaderSource = currentSource;
					currentSource = "";
					continue;
				}

				currentSource += line + "\n";
			}

			source.fragmentShaderSource = currentSource;
		}

		inputFile.close();
		return source;
	}
}