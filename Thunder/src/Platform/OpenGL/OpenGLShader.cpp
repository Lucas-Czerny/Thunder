#pragma once
#include "td_pch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Thunder
{
	OpenGLShader::OpenGLShader(const std::string& name, ShaderSource shaderSource)
		: m_Name(name)
	{
		CompileAndLinkShader(shaderSource);
	}

	void OpenGLShader::CompileAndLinkShader(ShaderSource shaderSource)
	{
		TD_PROFILE_FUNCTION();

		std::string vertexSource = shaderSource.vertexShaderSource,
			fragmentSource = shaderSource.fragmentShaderSource;

		uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const char* source = vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		glCompileShader(vertexShader);

		int32_t compilationStatus = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilationStatus);

		if (compilationStatus == 0)
		{
			int32_t length = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);

			char* infoLog = (char*)alloca(length);
			glGetShaderInfoLog(vertexShader, length, &length, &infoLog[0]);

			glDeleteShader(vertexShader);

			TD_CORE_ERROR("Vertex Shader {0} failed to compile!", m_Name);
			TD_CORE_ERROR("-> {0}", infoLog);

			TD_CORE_ASSERT(false, "");
			return;
		}

		uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationStatus);

		if (compilationStatus == 0)
		{
			int32_t length = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);

			char* infoLog = (char*)alloca(length);
			glGetShaderInfoLog(fragmentShader, length, &length, &infoLog[0]);

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			TD_CORE_ERROR("Fragment Shader {0} failed to compile!", m_Name);
			TD_CORE_ERROR("-> {0}", infoLog);

			TD_CORE_ASSERT(false, "");
			return;
		}

		m_RendererID = glCreateProgram();

		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		glLinkProgram(m_RendererID);

		int32_t linkingStatus = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int32_t*)&linkingStatus);

		if (linkingStatus == 0)
		{
			int32_t length = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &length);

			char* infoLog = (char*)alloca(length);
			glGetProgramInfoLog(m_RendererID, length, &length, &infoLog[0]);

			glDeleteProgram(m_RendererID);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return;
		}

		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		TD_PROFILE_FUNCTION();
		
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		TD_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}
	void OpenGLShader::Unbind() const
	{
		TD_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& uniformName, int value)
	{
		int32_t location = GetUniformLocation(uniformName);
		glUniform1i(location, value);
	}

	void OpenGLShader::SetIntArray(const std::string& uniformName, int* values, uint32_t count)
	{
		int32_t location = GetUniformLocation(uniformName);
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::SetFloat(const std::string& uniformName, float value)
	{
		int32_t location = GetUniformLocation(uniformName);
		glUniform1f(location, value);
	}

	void OpenGLShader::SetFloat2(const std::string& uniformName, glm::vec2 vector)
	{
		int32_t location = GetUniformLocation(uniformName);
		glUniform2f(location, vector.x, vector.y);
	}

	void OpenGLShader::SetFloat3(const std::string& uniformName, glm::vec3 vector)
	{
		int32_t location = GetUniformLocation(uniformName);
		glUniform3f(location, vector.x, vector.y, vector.z);
	}

	void OpenGLShader::SetFloat4(const std::string& uniformName, glm::vec4 vector)
	{
		int32_t location = GetUniformLocation(uniformName);
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}

	void OpenGLShader::SetMat3(const std::string& uniformName, glm::mat3 matrix)
	{
		int32_t location = GetUniformLocation(uniformName);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetMat4(const std::string& uniformName, glm::mat4 matrix)
	{
		int32_t location = GetUniformLocation(uniformName);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	
	int32_t OpenGLShader::GetUniformLocation(const std::string& uniformName)
	{
		auto loc = m_UniformCache.find(uniformName);
		if (loc == m_UniformCache.end())
		{
			int32_t location = glGetUniformLocation(m_RendererID, uniformName.c_str());
			m_UniformCache[uniformName] = location;
			return location;
		}

		return loc->second;
	}
}