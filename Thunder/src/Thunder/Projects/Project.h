#pragma once

#include "td_pch.h"

namespace Thunder
{
	struct Project
	{
		std::string Name = "";
		fs::path ProjectDirectory = "";
		fs::path StartupScenePath = "";

		fs::path GetProjectFilePath() const { return ProjectDirectory / ".thunder"; }
		fs::path GetAssetsPath() const { return ProjectDirectory / "assets"; }
	};
}
