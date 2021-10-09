#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Thunder
{
	struct ShaderSource
	{
		std::string vertexShaderSource, fragmentShaderSource;
	};

	class Shader
	{
	public:
		static Ref<Shader> Create(const std::string& shaderPath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

		virtual ~Shader() {}

		virtual std::string GetName() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& uniformName, int value) = 0;
		virtual void SetIntArray(const std::string& uniformName, int* values, uint32_t counter) = 0;

		virtual void SetFloat(const std::string& uniformName, float value) = 0;
		virtual void SetFloat2(const std::string& uniformName, glm::vec2 vector) = 0;
		virtual void SetFloat3(const std::string& uniformName, glm::vec3 vector) = 0;
		virtual void SetFloat4(const std::string& uniformName, glm::vec4 vector) = 0;

		virtual void SetMat3(const std::string& uniformName, glm::mat3 matrix) = 0;
		virtual void SetMat4(const std::string& uniformName, glm::mat4 matrix) = 0;

		virtual int32_t GetUniformLocation(const std::string& uniformName) = 0;

	private:
		static Ref<Shader> CreateImpl(const std::string& name, ShaderSource shaderSource);

		static ShaderSource GetShaderSourceFromFile(const std::string& shaderPath);
	};
}