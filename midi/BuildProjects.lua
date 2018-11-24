solution "gmcl_midi"

	language "C++"
	platforms {"x32", "x64"}
	location ( os.host() .. "-" .. _ACTION )
	symbols "On"
	editandcontinue "Off"
	staticruntime "On"
	vectorextensions "SSE"
	flags { "NoPCH" }
	targetdir ( "lib/" .. os.host() .. "/" )
	includedirs { "../include/" }

	configurations
	{
		"Release"
	}

	configuration "Release"
		defines { "NDEBUG" }
		optimize "On"
		floatingpoint "Fast"

	project "gmcl_midi"
		defines { "GMMODULE" }
		files { "src/**.*", "../include/**.*" }
		kind "SharedLib"

