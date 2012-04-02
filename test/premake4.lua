project "UnitTesting"
    kind "ConsoleApp"
    language "C++"

    targetdir "../bin"
    targetname "unit_tests"

    includedirs { "../include/Gecon" }
    includedirs { "../src" }
    includedirs { "../third-party/tut-2009-08-30" }
    includedirs { "../config/tut/" }

    files { "../config/tut/*.cpp" }
    files { "unit_tests/*.cpp" }

    links { "GeconFramework" }

project "ManualTesting"
    kind "ConsoleApp"
    language "C++"

    uses "QtGui"

    links {
        "boost_system",
        "boost_filesystem",
        "boost_thread"
    }

    targetdir "../bin"
    targetname "manual_tests"

    includedirs { "../include/Gecon" }
    includedirs { "../src" }
    includedirs { "../third-party/ManualTest/include" }

    files { "../third-party/ManualTest/src/*.cpp" }

    files { "manual_tests/*.hpp" }
    files { "manual_tests/*.cpp" }
    files { "manual_tests/*.ui" }
    files { "manual_tests/*.qrc" }

    links { "GeconFramework" }
