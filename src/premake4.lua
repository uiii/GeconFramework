project "GeconFramework"
    kind "SharedLib"
    language "C++"

    targetdir "../lib"

    links {
        "boost_filesystem",
        "boost_thread"
    }

    configuration { "linux or bsd or solaris" }
        require_pkg_config()
        require_pkg("libv4l2")
        files {
            "V4L2Error.cpp",
            "V4L2DeviceDescriptor.cpp",
            --"V4L2VideoDeviceAdapter.cpp",
            "V4L2VideoDeviceCapture.cpp",
            --"V4L2VideoDevicePolicy.cpp"
        }

    includedirs { "./" }
    includedirs { "../include/Gecon" }

    files {
        "ColorObjectPolicy.cpp",
        "Event.cpp",
        "Point.cpp"
    }
