workspace "Cober" 
	architecture "x64"
	startproject "Sandbox"

	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "libraries/ImGui"

project "Cober"
	location "Cober"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Cober/src/pch.cpp" 

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/include",
		"libraries/ImGui"
	}

	libdirs 
	{
		"%{prj.name}/lib",
		"libraries/ImGui"
	}

	links 
	{
		"SDL2",
		"SDL2main",
		"opengl32",
		"glew32",
		"ImGui"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"CB_PLATFORM_WINDOWS",
			"CB_BUILD_DLL",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<SDL_opengl.h>",
			"GL_GLEXT_PROTOTYPES=1"
		}

		postbuildcommands
		{
			"{COPY} %{cfg.buildtarget.relpath} ..\\bin\\" .. outputdir .. "\\Sandbox",
			"{COPY} ..\\libraries\\SDL\\lib\\x64\\SDL2.dll ..\\bin\\" .. outputdir .. "\\Sandbox",
			"{COPY} ..\\libraries\\Glew\\bin\\Release\\x64\\glew32.dll ..\\bin\\" .. outputdir .. "\\Sandbox"
		} 


	filter "configurations:Debug"
		defines "CB_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "CB_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "CB_DIST"
		buildoptions "/MD"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Cober/include",
		"Cober/src"
	}

	libdirs 
	{
		"Cober/lib"
	}

	links 
	{
		"Cober",
		"SDL2",
		"SDL2main",
		"opengl32",
		"glew32"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"CB_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "CB_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "CB_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "CB_DIST"
		optimize "On"
