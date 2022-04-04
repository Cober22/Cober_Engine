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

IncludeDir = {}
IncludeDir["SDL"] = "Cober/include/SDL"
IncludeDir["Glew"] = "Cober/include/Glew"
IncludeDir["glm"] = "Cober/include/glm"
IncludeDir["ImGui"] = "Cober/include/ImGui"
IncludeDir["stb_image"] = "Cober/include/stb_image"
IncludeDir["Assimp"] = "Cober/include/Assimp/include"

group "Dependencies"
	include "Cober/include/ImGui"
	include "Cober/include/Assimp"
group ""

project "Cober"
	location "Cober"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Cober/src/pch.cpp" 	

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/include/stb_image/**.h",
		"%{prj.name}/include/stb_image/**.cpp",
		"%{prj.name}/include/glm/glm/**.hpp",
		"%{prj.name}/include/glm/glm/**.inl"
	}
	
	defines 
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/include",
		"%{IncludeDir.SDL}",
		"%{IncludeDir.Glew}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Assimp}"
	}

	libdirs 
	{
		"%{prj.name}/lib",
		"%{IncludeDir.ImGui}"
	}

	links 
	{
		"SDL2",
		"SDL2main",
		"opengl32",
		"glew32s",
		"ImGui"
	}

	postbuildcommands
	{
		"{COPY} ..\\libraries\\SDL\\lib\\x64\\SDL2.dll/ ..\\bin\\" .. outputdir .. "\\Sandbox"
	}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"GLEW_STATIC",
			"CB_PLATFORM_WINDOWS",
			"CB_BUILD_DLL",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<SDL_opengl.h>",
			"GL_GLEXT_PROTOTYPES=1"
		}

	filter "configurations:Debug"
		defines "CB_DEBUG"
		-- buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "CB_RELEASE"
		-- buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "CB_DIST"
		-- buildoptions "/MD"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Cober/src",
		"%{IncludeDir.glm}"
	}

	links 
	{
		"Cober"
	}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"CB_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "CB_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "CB_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "CB_DIST"
		optimize "on"
