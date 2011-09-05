builddir = "build"

solution "GeconFramework"
    configurations { "Debug", "Release" }

    location ( builddir )
    targetdir ( builddir )

    flags { "ExtraWarnings" }

    configuration { "linux or bsd or solaris" }
        defines { "UNIX" }

    configuration { "windows" }
        defines { "WINDOWS" }

    configuration { "linux", "gmake" }
        buildoptions { "-std=c++0x" }

    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "Release"
        flags { "Optimize" }

    project "GeconFramework"
        kind "SharedLib"
        language "C++"

        includedirs { "src" }

        files { "src/*.cpp" }

    project "UnitTesting"
        kind "ConsoleApp"
        language "C++"

        targetname "unit_tests"

        includedirs { "src" }
        includedirs { "lib/tut-2009-08-30" }

        files { "test/*.cpp" }

        links { "GeconFramework" }
