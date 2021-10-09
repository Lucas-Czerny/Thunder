#pragma once

#include "Project.h"

namespace Thunder
{
	class ProjectSerializer
	{
	private:
		Project& m_Project;

	public:
		ProjectSerializer(Project& project)
			: m_Project(project) {}

		void SerializeYAML(const fs::path& outputPath);
		void DeserializeYAML(const fs::path& inputPath);

		void SerializeBinary();
		void DeserializeBinary();
	};
}


