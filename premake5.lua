workspace "Airi"
    architecture "x64"
    configurations {"Debug", "Release"}

project "Airi"
    kind "ConsoleApp"
	
    files {
		"src/*.cpp",
		"inc/*.hpp"
	}

    includedirs { "inc" }