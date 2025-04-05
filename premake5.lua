require "Engine/Build/PremakeVSCode/vscode"

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
IncludeDir["GLFW"] = "Engine/Source/ThirdParty/glfw/include"
IncludeDir["GLAD"] = "Engine/Source/ThirdParty/glad/include"
IncludeDir["IMGUI"] = "Engine/Source/ThirdParty/imgui"
IncludeDir["GLM"] = "Engine/Source/ThirdParty/GLM"
IncludeDir["STB"] = "Engine/Source/ThirdParty/stb"

group "Dependencies"
	include "Engine/Source/ThirdParty/glfw"
	include "Engine/Source/ThirdParty/glad"
	include "Engine/Source/ThirdParty/imgui"
	include "Engine/Source/ThirdParty/stb"
group ""

project "Acrylic"
	location "Acrylic"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("Engine/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Engine/Intermediates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Engine/Source/Runtime/*.h",
		"Engine/Source/Runtime/*.hpp",
		"Engine/Source/Runtime/*.cpp",
		"Engine/Source/Runtime/**.h",
		"Engine/Source/Runtime/**.hpp",
		"Engine/Source/Runtime/**.cpp"
	}

	includedirs
	{
		"Engine/Source/Runtime",
		"Engine/Source/Runtime/**",
		"Engine/Source/ThirdParty",
		"Engine/Source/ThirdParty/**",
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
	location "AcrylicEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("Engine/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Engine/Intermediates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Engine/Source/Editor/*.h",
		"Engine/Source/Editor/*.hpp",
		"Engine/Source/Editor/*.cpp",
		"Engine/Source/Editor/**.h",
		"Engine/Source/Editor/**.hpp",
		"Engine/Source/Editor/**.cpp"
	}

	includedirs 
	{
		"Engine/Source/Editor",
		"Engine/Source/Editor/**",
		"Engine/Source/Runtime",
		"Engine/Source/Runtime/**",
		"Engine/Source/ThirdParty",
		"Engine/Source/ThirdParty/**",
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