project "GeconFramework"
    kind "SharedLib"
    language "C++"

    configuration { "linux or bsd or solaris" }
        require_pkg_config()
        require_pkg("libv4l2")
        files {
            "CameraManager_v4l2.cpp"
        }

    includedirs { "./" }

    files {
        "Camera.cpp",
        "CameraDevice.cpp",
        "CameraManager.cpp",
        "CameraManager_v4l2.cpp",
        "Color.cpp",
        "Image.cpp",
        "V4l2CameraDevice.cpp"
    }
