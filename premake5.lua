workspace "Acrylic"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Acrylic/src/Acrylic/ThirdParty/glfw/include"

include "Acrylic/src/Acrylic/ThirdParty/glfw"

project "Acrylic"
	location "Acrylic"
	kind "SharedLib"
	language "C++"

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

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/%{prj.name}/Editor",
		"%{prj.name}/src/%{prj.name}/Runtime",
		"%{prj.name}/src/%{prj.name}/ThirdParty",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "10.0.22621.0"

		defines
		{
			"AC_PLATFORM_WINDOWS",
			"AC_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines 
		{
			"AC_DEBUG",
			"AC_ENABLE_ASSERTS"
		}
		symbols "On"

	filter "configurations:Release"
		defines "AC_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "AC_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		staticruntime "On"
		systemversion "10.0.22621.0"

		defines
		{
			"AC_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "AC_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "AC_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "AC_DIST"
		optimize "On"