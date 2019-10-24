workspace ("Primordial")
    location("./")

    defines "_CRT_SECURE_NO_WARNINGS"
    configurations { "Debug", "Release" }
	platforms "x64"
	
	includedirs {
		os.getenv("CappuccinoPath") .. "/Cappuccino/include",
		os.getenv("CappuccinoPath") .. "/Externals/fmod/include",
		os.getenv("CappuccinoPath") .. "/Externals/freetype/include",
		os.getenv("CappuccinoPath") .. "/Externals/glad/include",
		os.getenv("CappuccinoPath") .. "/Externals/glfw3/include",
		os.getenv("CappuccinoPath") .. "/Externals/glm/include",
		os.getenv("CappuccinoPath") .. "/Externals/imgui/include",
        os.getenv("CappuccinoPath") .. "/Externals/stb/include",
        "./include"
	}

	filter "platforms:x64"
		system "Windows"
		architecture "x64"
		defines "Win64"
    
    libdirs {
		os.getenv("CappuccinoPath") .. "/Build/bin/%{cfg.architecture}/%{cfg.buildcfg}",
		os.getenv("CappuccinoPath") .. "/Externals/Build/bin/%{cfg.architecture}/%{cfg.buildcfg}"
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