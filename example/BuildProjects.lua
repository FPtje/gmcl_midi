solution "gmcl_midi"

	language "C++"
	location ( os.get() .."-".. _ACTION )
	flags { "Symbols", "NoEditAndContinue", "NoPCH", "StaticRuntime", "EnableSSE" }
	targetdir ( "lib/" .. os.get() .. "/" )
	includedirs { "../include/" }

	configurations
	{
		"Release"
	}

	configuration "Release"
		defines { "NDEBUG" }
		flags{ "Optimize", "FloatFast" }

	project "gmcl_midi"
		defines { "GMMODULE" }
		files { "src/**.*", "../include/**.*" }
		kind "SharedLib"

