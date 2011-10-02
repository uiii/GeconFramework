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

project "ManualTesting"
    kind "ConsoleApp"
    language "C++"

    targetname "manual_tests"

    includedirs { "../src" }
    includedirs { "../config/manual_tests" }

    files { "../config/manual_tests/*.cpp" }
    files { "manual_tests/*.cpp" }

    links { "GeconFramework" }
