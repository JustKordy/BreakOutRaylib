workspace "BreakOutRaylibKordos"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "App"

   SolutionDir = path.getabsolute(".")

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
	include "Core/Build-Core.lua"
group ""

include "App/Build-App.lua"

-- Global include directories for all projects if necessary
IncludeDir = {}
IncludeDir["Raylib"] = "$(SolutionDir)Vendor/Raylib/include"

-- Global library directories if necessary
LibDir = {}
LibDir["Raylib"] = "$(SolutionDir)Vendor/Raylib/lib"

project "App"
   location "App"
   kind "ConsoleApp"
   language "C++"
   targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")

   files { "App/src/**.h", "App/src/**.cpp" }

   -- Include directories for Raylib dependency
   includedirs
   {
      "%{IncludeDir.Raylib}",  -- Include Raylib headers
   }

   -- Library directories for Raylib dependency
   libdirs
   {
      "%{LibDir.Raylib}",  -- Link against Raylib library
   }

   -- Link with the Raylib library
   links
   {
      "raylib", -- The actual library name, like raylib.lib on Windows
      "winmm"
   }

   filter "system:windows"
      cppdialect "C++17"
      systemversion "latest"

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
