outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "Juniper"
    architecture "x64"
    startproject "JuniperEditor"
    configurations { "Debug", "Release" }

group "Vendor"
    include "Juniper/vendor/glfw"
    include "Juniper/vendor/glad"
    include "Juniper/vendor/imgui"

group ""

project "Juniper"
    location "Juniper"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "%{prj.name}/src/pch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glad/src/glad.c",
        "%{prj.name}/vendor/stb_image/**.h", "%{prj.name}/vendor/stb_image/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/glfw/include",
        "%{prj.name}/vendor/glad/include",
        "%{prj.name}/vendor/imgui",
        "%{prj.name}/vendor/glm",
        "%{prj.name}/vendor/stb_image",
        "%{prj.name}/vendor/entt",
    }

    links { "GLFW", "glad", "opengl32.lib", "ImGui" }

    -- Systems
    filter "system:windows"
        systemversion "latest"
        defines { "JP_PLATFORM_WINDOWS", "JP_BUILD_DLL" }
        buildoptions { "/utf-8" }

    -- Configurations
    filter "configurations:Debug"
        defines { "JP_DEBUG", "JP_ASSERTS", "GLFW_INCLUDE_NONE" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "JP_RELEASE", "GLFW_INCLUDE_NONE" } 
        runtime "Release"
        optimize "on"

    -- Files
    filter "files:Juniper/vendor/glad/src/glad.c"
        flags { "NoPCH" }

project "Application"
    location "Application"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outdir .. "/%{prj.name}")

    files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }

    includedirs {
        "Juniper/vendor/spdlog/include",
        "Juniper/vendor/glfw/include",
        "Juniper/vendor/glad/include",
        "Juniper/src",
        "Juniper/vendor/glm",
        "Juniper/vendor/entt",
        "Juniper/vendor"
    }

    links { "Juniper" }

    filter "system:windows"
        staticruntime "On"
        systemversion "latest"
        defines { "JP_PLATFORM_WINDOWS", "GLFW_INCLUDE_NONE" }
        buildoptions { "/utf-8" }

    filter "configurations:Debug"
        defines { "JP_DEBUG", "JP_ASSERTS" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "JP_RELEASE", "GLFW_INCLUDE_NONE" }
        runtime "Release"
        optimize "on"

project "JuniperEditor"
    location "JuniperEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outdir .. "/%{prj.name}")

    files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }

    includedirs {
        "Juniper/vendor/spdlog/include",
        "Juniper/vendor/glfw/include",
        "Juniper/vendor/glad/include",
        "Juniper/src",
        "Juniper/vendor/glm",
        "Juniper/vendor/entt",
        "Juniper/vendor"
    }

    links { "Juniper" }

    filter "system:windows"
        staticruntime "On"
        systemversion "latest"
        defines { "JP_PLATFORM_WINDOWS", "GLFW_INCLUDE_NONE" }
        buildoptions { "/utf-8" }

    filter "configurations:Debug"
        defines { "JP_DEBUG", "JP_ASSERTS" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "JP_RELEASE", "GLFW_INCLUDE_NONE" }
        runtime "Release"
        optimize "on"
