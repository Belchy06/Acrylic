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

group "Dependencies"
	include "Engine/Source/ThirdParty/glad"
	include "Engine/Source/ThirdParty/glfw"
	include "Engine/Source/ThirdParty/glm"
	include "Engine/Source/ThirdParty/imgui"
	include "Engine/Source/ThirdParty/stb"
group ""

project "Acrylic"
	print ("Adding project: Acrylic")
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
		"Engine/Source/ThirdParty/**"
	}

	directories = os.matchdirs("Engine/Source/ThirdParty/*")
	libs = {}
	for dirCount = 1, #directories do
		local dir = directories[dirCount]
		table.insert(libs, path.getname(dir))
	end
	links { "opengl32.lib", libs }

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
	print ("Adding project: AcrylicEditor")
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
		"Engine/Source/ThirdParty/**"
	}

	directories = os.matchdirs("Engine/Source/ThirdParty/*")
	libs = {}
	for dirCount = 1, #directories do
		local dir = directories[dirCount]
		table.insert(libs, path.getname(dir))
	end
	links { "Acrylic", "opengl32.lib", libs }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"AC_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines { "AC_DEBUG", "WITH_EDITOR" }
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines { "AC_RELEASE", "WITH_EDITOR" }
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines { "AC_DIST", "WITH_EDITOR" }
		optimize "on"
		runtime "Release"