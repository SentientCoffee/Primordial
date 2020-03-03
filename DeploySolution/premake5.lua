workspace ("Primordial")
    location("./")

    defines "_CRT_SECURE_NO_WARNINGS"
    configurations { "Debug", "Release" }
	platforms "x64"
	
	includedirs {
		"./Engine",
		"./Engine/Externals/fmod/include",
		"./Engine/Externals/fmod/studio/include",
		"./Engine/Externals/freetype/include",
		"./Engine/Externals/glad/include",
		"./Engine/Externals/glfw3/include",
		"./Engine/Externals/glm/include",
		"./Engine/Externals/imgui/include",
        "./Engine/Externals/stb/include",
        "./include"
	}

	filter "platforms:x64"
		system "Windows"
		architecture "x64"
		defines "Win64"
    
    libdirs {
		"libs/%{cfg.buildcfg}",
    }
    
    filter "platforms:x64"
		system "windows"
		architecture "x64"
		defines "Win64"
	
	filter "system:windows"
		systemversion "latest"
		
		defines {
			"GLFW_INCLUDE_NONE",
			"WINDOWS"
		}

		linkoptions {
			"/subsystem:windows",
			"/entry:mainCRTStartup"
		}
		
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
        optimize "on"
		
		filter{}

 project ("Primordial")
	kind "ConsoleApp"
	configuration "windows"
	targetname ("Primordial")

	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("./Build/bin/%{cfg.platform}/%{cfg.buildcfg}")
    objdir    ("./Build/obj/%{cfg.platform}/%{cfg.buildcfg}")

	
	links {
		"capp",
		"Externals"
	}
	
	files {
		"./include/**.h",
		"./include/**.hpp",
		"./src/**.cpp",
		"./Assets/**"
	}