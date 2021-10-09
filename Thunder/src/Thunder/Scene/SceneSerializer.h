#pragma once

#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Entity.h"

namespace fs = std::filesystem;

namespace Thunder
{
	class SceneSerializer
	{
	private:
		Ref<Scene> m_Scene;

	public:
		SceneSerializer(const Ref<Scene>& scene)
			: m_Scene(scene) {}

		void SetActiveScene(const Ref<Scene>& scene) { m_Scene = scene; }

		void SerializeYAML(const fs::path& outputPath);
		void DeserializeYAML(const fs::path& inputPath);

		void SerializeBinary();
		void DeserializeBinary();

	private:
		static void SerializeEntityYAML(YAML::Emitter& out, Entity& entity);
		static void DeserializeEntityYAML(const YAML::Node& entityNode, Entity& entity);
	};
}


