workspace "MonteCarloPathTracing"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include_directions = {}
include_directions["spdlog"] = "%{wks.location}/MonteCarloPathTracing/vender/spdlog/include"
include_directions["stb"] = "%{wks.location}/MonteCarloPathTracing/vender/stb"
include_directions["tinyxml2"] = "%{wks.location}/MonteCarloPathTracing/vender/tinyxml2"

project "MonteCarloPathTracing"
    location "MonteCarloPathTracing"
    kind "ConsoleApp"
    language "C++"
    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin_intermediate/" .. output_dir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vender/tinyxml2/tinyxml2.h",
        "%{prj.name}/vender/tinyxml2/tinyxml2.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{include_directions.spdlog}",
        "%{include_directions.stb}",
        "%{include_directions.tinyxml2}"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "GQY_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "GQY_MONTE_CARLO_PATH_TRACING_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "GQY_MONTE_CARLO_PATH_TRACING_RELEASE"
        optimize "On"