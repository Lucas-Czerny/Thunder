#pragma once

#include "Shader.h"

namespace Thunder
{
	class ShaderLibrary
	{
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;

	public:
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Get(const std::string& name);

	private:
		bool Exists(const std::string& name);
	};
}

