workspace "Acrylic"
	architecture "x64"

	startproject "AcrylicEditor"

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
IncludeDir["STB"] = "Acrylic/src/Acrylic/ThirdParty/stb"

group "Dependencies"
	include "Acrylic/src/Acrylic/ThirdParty/glfw"
	include "Acrylic/src/Acrylic/ThirdParty/glad"
	include "Acrylic/src/Acrylic/ThirdParty/imgui"
	include "Acrylic/src/Acrylic/ThirdParty/stb"
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
		"Acrylic/src/acpch.h",
		"Acrylic/src/acpch.cpp",
		"Acrylic/src/Acrylic.h",
		"Acrylic/src/Acrylic/Runtime/*.h",
		"Acrylic/src/Acrylic/Runtime/*.hpp",
		"Acrylic/src/Acrylic/Runtime/*.cpp",
		"Acrylic/src/Acrylic/Runtime/**.h",
		"Acrylic/src/Acrylic/Runtime/**.hpp",
		"Acrylic/src/Acrylic/Runtime/**.cpp"
	}

	includedirs
	{
		"Acrylic/src",
		"Acrylic/src/Acrylic/Runtime",
		"Acrylic/src/Acrylic/ThirdParty",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.IMGUI}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.STB}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"opengl32.lib",
		"ImGui",
		"STB"
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

project "AcrylicEditor"
	location "Acrylic"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Acrylic/src/Acrylic/Editor/*.h",
		"Acrylic/src/Acrylic/Editor/*.hpp",
		"Acrylic/src/Acrylic/Editor/*.cpp",
		"Acrylic/src/Acrylic/Editor/**.h",
		"Acrylic/src/Acrylic/Editor/**.hpp",
		"Acrylic/src/Acrylic/Editor/**.cpp"
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
		"%{IncludeDir.GLM}",
		"%{IncludeDir.STB}"
	}

	links
	{
		"Acrylic",
		"GLFW",
		"GLAD",
		"opengl32.lib",
		"ImGui",
		"STB"
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