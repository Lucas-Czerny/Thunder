#include "td_pch.h"
#include "ProjectSerializer.h"

#include <yaml-cpp/yaml.h>

namespace Thunder
{
	void ProjectSerializer::SerializeYAML(const fs::path& outputPath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap; // Project

		out << YAML::Key << "Project Name" << YAML::Value << m_Project.Name;
		out << YAML::Key << "Project Directory" << YAML::Value << m_Project.ProjectDirectory.string();
		out << YAML::Key << "Startup Scene Path" << YAML::Value << m_Project.StartupScenePath.string();

		out << YAML::EndMap; // Project

		std::ofstream outputFile(outputPath);
		outputFile << out.c_str();
	}

	void ProjectSerializer::DeserializeYAML(const fs::path& inputPath)
	{
		std::ifstream inputFile(inputPath);

		std::stringstream fileName;
		fileName << inputFile.rdbuf();

		inputFile.close();

		YAML::Node data = YAML::Load(fileName.str());

		m_Project.Name = data["Project Name"].as<std::string>();
		m_Project.ProjectDirectory = data["Project Directory"].as<std::string>();
		m_Project.StartupScenePath = data["Startup Scene Path"].as<std::string>();
	}

	void ProjectSerializer::SerializeBinary()
	{
		TD_CORE_ASSERT(false, "Not implemented yet, use SerializeYAML instead");
	}

	void ProjectSerializer::DeserializeBinary()
	{
		TD_CORE_ASSERT(false, "Not implemented yet, use DeserializeYAML instead");
	}
}
