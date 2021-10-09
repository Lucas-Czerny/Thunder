project "yaml-cpp"
	kind "StaticLib"
	language "C++"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}
	
	includedirs
	{
		"include"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
			
	filter "configurations:Release"
		runtime "Release"
		optimize "on"