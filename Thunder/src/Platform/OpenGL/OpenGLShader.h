#pragma once

#include "Thunder/Renderer/Shader.h"

namespace Thunder
{
	class OpenGLShader : public Shader
	{
	private:
		std::string m_Name;
		uint32_t m_RendererID;
		std::unordered_map<std::string, int32_t> m_UniformCache;

	public:
		OpenGLShader(const std::string& name, ShaderSource shaderSource);
		~OpenGLShader();

		std::string GetName() const override { return m_Name; }

		void CompileAndLinkShader(ShaderSource shaderSource);

		void Bind() const override;
		void Unbind() const override;

		void SetInt(const std::string& uniformName, int value) override;
		void SetIntArray(const std::string& uniformName, int* values, uint32_t counter) override;

		void SetFloat(const std::string& uniformName, float value) override;
		void SetFloat2(const std::string& uniformName, glm::vec2 vector) override;
		void SetFloat3(const std::string& uniformName, glm::vec3 vector) override;
		void SetFloat4(const std::string& uniformName, glm::vec4 vector) override;

		void SetMat3(const std::string& uniformName, glm::mat3 matrix) override;
		void SetMat4(const std::string& uniformName, glm::mat4 matrix) override;

		int32_t GetUniformLocation(const std::string& uniformName) override;
	};
}