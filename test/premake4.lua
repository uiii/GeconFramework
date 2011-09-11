project "UnitTesting"
    kind "ConsoleApp"
    language "C++"

    targetname "unit_tests"

    includedirs { "../src" }
    includedirs { "../lib/tut-2009-08-30" }
    includedirs { "../config/tut/" }

    files { "../config/tut/*.cpp" }
    files { "unit_tests/*.cpp" }

    links { "GeconFramework" }
