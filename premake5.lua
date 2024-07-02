workspace "Acrylic"
	architecture "x64"

	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Acrylic/src/Acrylic/ThirdParty/glfw/include"
IncludeDir["GLAD"] = "Acrylic/src/Acrylic/ThirdParty/glad/include"
IncludeDir["IMGUI"] = "Acrylic/src/Acrylic/ThirdParty/imgui"
IncludeDir["GLM"] = "Acrylic/src/Acrylic/ThirdParty/GLM"

group "Dependencies"
	include "Acrylic/src/Acrylic/ThirdParty/glfw"
	include "Acrylic/src/Acrylic/ThirdParty/glad"
	include "Acrylic/src/Acrylic/ThirdParty/imgui"
group ""

project "Acrylic"
	location "Acrylic"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

	pchheader "acpch.h"
	pchsource "Acrylic/src/acpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	removefiles
	{
		"%{prj.name}/src/Acrylic/ThirdParty/**.h",
		"%{prj.name}/src/Acrylic/ThirdParty/**.hpp",
		"%{prj.name}/src/Acrylic/ThirdParty/**.cpp",
		"%{prj.name}/src/Acrylic/ThirdParty/**.cppm",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/%{prj.name}/Editor",
		"%{prj.name}/src/%{prj.name}/Runtime",
		"%{prj.name}/src/%{prj.name}/ThirdParty",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.IMGUI}",
		"%{IncludeDir.GLM}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"opengl32.lib",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"AC_PLATFORM_WINDOWS",
			"AC_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "AC_DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "AC_RELEASE"
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "AC_DIST"
		optimize "on"
		runtime "Release"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"Acrylic/src",
		"Acrylic/src/Acrylic/Editor",
		"Acrylic/src/Acrylic/Runtime",
		"Acrylic/src/Acrylic/ThirdParty",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.IMGUI}",
		"%{IncludeDir.GLM}"
	}

	links
	{
		"Acrylic",
		"GLFW",
		"GLAD",
		"opengl32.lib",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"AC_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "AC_DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "AC_RELEASE"
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "AC_DIST"
		optimize "on"
		runtime "Release"