workspace "Cober" 
	architecture "x64"
	startproject "CoberEditor"

	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
--IncludeDir["SDL"] = "Cober/include/SDL"
IncludeDir["Glew"] = "Cober/include/Glew"
IncludeDir["glm"] = "Cober/include/glm"
IncludeDir["ImGui"] = "Cober/include/ImGui"
IncludeDir["stb_image"] = "Cober/include/stb_image"
IncludeDir["Assimp"] = "Cober/include/assimp"
IncludeDir["GLFW"] = "Cober/include/GLFW"

group "Dependencies"
	include "Cober/include/ImGui"
group ""

project "Cober"
	location "Cober"
	kind "StaticLib"
	language "C++"
	warnings "Off"
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
		"%{IncludeDir.Glew}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.GLFW}"
	}

	libdirs 
	{
		"%{prj.name}/lib"
	}

	links 
	{
		"opengl32",
		"glew32s",
		"ImGui",
		"glfw3_mt",
		"assimp-vc142-mt"
	}

	postbuildcommands
	{
		"{COPY} ..\\%{prj.name}\\lib\\assimp-vc142-mt.dll/ ..\\bin\\" .. outputdir .. "\\CoberEditor"
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

project "CoberEditor"
	location "CoberEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
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
