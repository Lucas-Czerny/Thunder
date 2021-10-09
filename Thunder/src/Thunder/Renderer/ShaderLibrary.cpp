#include "td_pch.h"
#include "ShaderLibrary.h"

namespace Thunder
{
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		std::string name = shader->GetName();

		TD_CORE_ASSERT(!Exists(name), "Shader already exists!");

		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);

		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		Ref<Shader> shader = m_Shaders[name];
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}