workspace "Thunder"
	architecture "x64"
	startproject "Thunderbird"	

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Thunder/vendor/GLFW/include"
IncludeDir["Glad"] = "Thunder/vendor/Glad/include"
IncludeDir["ImGui"] = "Thunder/vendor/imgui"
IncludeDir["glm"] = "Thunder/vendor/glm"
IncludeDir["stb_image"] = "Thunder/vendor/stb_image"
IncludeDir["entt"] = "Thunder/vendor/EnTT/include"
IncludeDir["yaml_cpp"] = "Thunder/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "Thunder/vendor/ImGuizmo"
IncludeDir["Box2D"] = "Thunder/vendor/Box2D/include"

group "Dependencies"
	include "Thunder/vendor/GLFW"
	include "Thunder/vendor/Glad"
	include "Thunder/vendor/imgui"
	include "Thunder/vendor/yaml-cpp"
	include "Thunder/vendor/Box2D"
group ""

project "Thunder"
	location "Thunder"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "td_pch.h"
	pchsource "Thunder/src/td_pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{IncludeDir.ImGuizmo}/ImGuizmo.h",
		"%{IncludeDir.ImGuizmo}/ImGuizmo.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Box2D}"
	}

	links
	{
		"GLFW",
		"ImGui",
		"Glad",
		"yaml-cpp",
		"opengl32.lib",
		"Box2D"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TD_PLATFORM_WINDOWS",
			"TD_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "TD_DEBUG"
		runtime "Debug"
		symbols "on"
			
	filter "configurations:Release"
		defines "TD_RELEASE"
		runtime "Release"
		optimize "on"
			
	filter "configurations:Dist"
		defines "TD_DIST"
		runtime "Release"
		optimize "on"

project "Thunderbird"
	location "Thunderbird"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Thunder/vendor/spdlog/include",
		"Thunder/src",
		"Thunder/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"Thunder"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TD_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "TD_DEBUG"
		runtime "Debug"
		symbols "on"
			
	filter "configurations:Release"
		defines "TD_RELEASE"
		runtime "Release"
		optimize "on"
			
	filter "configurations:Dist"
		defines "TD_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Thunder/vendor/spdlog/include",
		"Thunder/src",
		"Thunder/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}"
	}

	links
	{
		"Thunder"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TD_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "TD_DEBUG"
		runtime "Debug"
		symbols "on"
			
	filter "configurations:Release"
		defines "TD_RELEASE"
		runtime "Release"
		optimize "on"
			
	filter "configurations:Dist"
		defines "TD_DIST"
		runtime "Release"
		optimize "on"
		
filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }