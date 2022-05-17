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
IncludeDir["ImGuizmo"] = "Cober/include/ImGuizmo"
IncludeDir["stb_image"] = "Cober/include/stb_image"
IncludeDir["Assimp"] = "Cober/include/assimp"
IncludeDir["GLFW"] = "Cober/include/GLFW"
IncludeDir["ENTT"] = "Cober/include/ENTT"
IncludeDir["yaml"] = "Cober/include/yaml/include"
IncludeDir["Bullet"] = "Cober/include/Bullet"
IncludeDir["Box2D"] = "Cober/include/Box2D/include"
IncludeDir["Fmod"] = "Cober/include/Fmod"

group "Dependencies"
	include "Cober/include/ImGui"
	include "Cober/include/yaml"
	include "Cober/include/Box2D"
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
		"%{prj.name}/include/glm/glm/**.inl",
		"%{prj.name}/include/ENTT/**.hpp",
		"%{prj.name}/include/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/include/ImGuizmo/ImGuizmo.cpp",
		"%{prj.name}/include/Fmod/**.hpp",
		"%{prj.name}/include/Bullet/BulletCollision/**.h",
		"%{prj.name}/include/Bullet/BulletCollision/**.cpp",
		"%{prj.name}/include/Bullet/BulletDynamics/**.h",
		"%{prj.name}/include/Bullet/BulletDynamics/**.cpp",
		"%{prj.name}/include/Bullet/LinearMath/**.h",
		"%{prj.name}/include/Bullet/LinearMath/**.cpp",
		"%{prj.name}/include/Box2D/include/box2d/**.h",
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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ENTT}",
		"%{IncludeDir.yaml}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Bullet}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.Fmod}"
	}

	libdirs 
	{
		"%{prj.name}/lib",
		"%{prj.name}/lib/Bullet",
		"%{prj.name}/lib/Fmod"
	}

	links 
	{
		"opengl32",
		"glew32s",
		"ImGui",
		"glfw3_mt",
		"assimp-vc142-mt",
		"yaml-cpp",
		"Box2D",
		"fmodL_vc",
		"fmod_vc",

	}

	postbuildcommands
	{
		"{COPY} ..\\%{prj.name}\\lib\\assimp-vc142-mt.dll/ ..\\bin\\" .. outputdir .. "\\CoberEditor"
	}

	filter "files:Cober/include/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "files:Cober/include/Bullet/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"GLEW_STATIC",
			"CB_PLATFORM_WINDOWS",
			"CB_BUILD_DLL",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<SDL_opengl.h>",
			"GL_GLEXT_PROTOTYPES=1",
			"YAML_CPP_STATIC_DEFINE"
		}

	filter "configurations:Debug"
		defines "CB_DEBUG"
		--buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "CB_RELEASE"
		--buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "CB_DIST"
		--buildoptions "/MD"
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.ENTT}"
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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/include/ENTT/**.hpp"
	}

	includedirs
	{
		"Cober/include",
		"Cober/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ENTT}"
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
