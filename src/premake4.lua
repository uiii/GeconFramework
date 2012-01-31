project "GeconFramework"
    kind "SharedLib"
    language "C++"

    configuration { "linux or bsd or solaris" }
        require_pkg_config()
        require_pkg("libv4l2")
        files {
        }

    includedirs { "./" }

    files {
        "Control.hpp",
        "DeviceManager.cpp"
    }
