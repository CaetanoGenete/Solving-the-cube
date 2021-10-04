workspace "Solving the cube"

architecture "x86"
configurations {"Debug", "Release"}

project "Solving the cube"
	location "%{prj.name}"

	language "C++"
	cppdialect "C++14"

	kind "ConsoleApp"

	targetdir ("%{prj.name}/build/bin/%{prj.name}-%{cfg.buildcfg}-%{cfg.architecture}" )
	objdir ("%{prj.name}/build/obj/%{prj.name}-%{cfg.buildcfg}-%{cfg.architecture}" )

	defines {
		"GLEW_STATIC"
	}

	files {
		"%{prj.name}/**.cpp",
		"%{prj.name}/**.h"
	}
	removefiles {
		"%{prj.name}/ImGUI/main.cpp"
	}

	includedirs {
		"Dependencies/CUTILS/include",
		"Dependencies/GLEW/include",
		"Dependencies/GLFW/include",
		"Dependencies/GLM"
	}

	libdirs {
		"Dependencies/CUTILS/lib",
		"Dependencies/GLEW/lib/Release/Win32",
		"Dependencies/GLFW/lib-vc2015"
	}

	links {
		"glew32s.lib",
		"glfw3.lib",
		"opengl32.lib",
		"glu32.lib"
	}

	filter {"configurations:Debug"}
		symbols "On"

		links{"cutilsDEBUG.lib"}

	filter {"configurations:Release"}
		optimize "On"

		links{"cutils.lib"}

	