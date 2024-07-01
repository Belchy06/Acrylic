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

include "Acrylic/src/Acrylic/ThirdParty/glfw"
include "Acrylic/src/Acrylic/ThirdParty/glad"
include "Acrylic/src/Acrylic/ThirdParty/imgui"

project "Acrylic"
	location "Acrylic"
	kind "SharedLib"
	language "C++"
	staticruntime "Off"

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
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/%{prj.name}/Editor",
		"%{prj.name}/src/%{prj.name}/Runtime",
		"%{prj.name}/src/%{prj.name}/ThirdParty",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.IMGUI}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"

		defines
		{
			"AC_PLATFORM_WINDOWS",
			"AC_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines 
		{
			"AC_DEBUG"
		}
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "AC_RELEASE"
		optimize "On"
		runtime "Release"

	filter "configurations:Dist"
		defines "AC_DIST"
		optimize "On"
		runtime "Release"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "Off"

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
		"Acrylic/src/Acrylic/ThirdParty"
	}

	links
	{
		"Acrylic"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"

		defines
		{
			"AC_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "AC_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "AC_RELEASE"
		optimize "On"
		runtime "Release"

	filter "configurations:Dist"
		defines "AC_DIST"
		optimize "On"
		runtime "Release"