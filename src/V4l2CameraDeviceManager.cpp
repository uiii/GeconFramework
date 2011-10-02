/**
 * This file is part of the Gecon Framework project (https://github.com/uiii/Gecon-Framework).
 *
 * Copyright (C) 2011 by Richard Jedlička <uiii.dev@gmail.com>
 *
 * Gecon Framework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gecon Framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Gecon Framework. If not, see <http://www.gnu.org/licenses/>.
 */

#include "V4l2CameraDeviceManager.hpp"

#include <iostream>

#include <list>
#include <string>

#include <stdio.h>
#include <dirent.h>
#include <error.h>
#include <errno.h>

#include <algorithm>

#include <sys/stat.h>
#include <fcntl.h>

#include <system_error>

#include "V4l2CameraDevice.hpp"

Gecon::V4l2CameraDeviceManager::V4l2CameraDeviceManager()
{
}

Gecon::CameraDevicePtrList Gecon::V4l2CameraDeviceManager::getAvailableCameraDevices()
{
    Gecon::CameraDevicePtrList deviceList;

    Gecon::V4l2DeviceFileList deviceFiles = listDeviceFiles_();
    for(const Gecon::V4l2DeviceFile& deviceFile : deviceFiles)
    {
        Gecon::V4l2CameraDevicePtr device;

        auto it = devices_.find(deviceFile.file);

        if(it != devices_.end())
        {
            it->second->updateDeviceFile(deviceFile);
            device = it->second;
        }
        else
        {
            device = std::make_shared<Gecon::V4l2CameraDevice>(deviceFile);
            devices_[deviceFile.file] = device;
        }

        deviceList.push_back(device);
    }

    return deviceList;
}

Gecon::V4l2DeviceFileList Gecon::V4l2CameraDeviceManager::listDeviceFiles_() const
{
    Gecon::StringList dirList = { "/devv/", "/dev/v4l/" };

    Gecon::V4l2DeviceFileList deviceFiles;

    for(const std::string& dirPath : dirList)
    {
        std::map<std::string, Gecon::StringList> dirDeviceFilesAndSymlinks;

        DIR* dir = opendir(dirPath.c_str());
        if(!dir)
        {
            std::string error = "Cannot open '" + dirPath + "' directory";
            //std::cout << std::system_category().message(errno) << std::endl;
            // TODO print error

            exit(1);
        }

        dirent* entry;
        while((entry = readdir(dir)))
        {
            std::string fileName = dirPath + entry->d_name;

            if(isVideoDevice_(fileName))
            {
                char linkTarget[4096];
                if(readlink(fileName.c_str(), linkTarget, 4096) > 0) // fileName is symlink
                {
                    if(linkTarget[0] == '/')
                    {
                        dirDeviceFilesAndSymlinks[linkTarget].push_back(fileName);
                    }
                    else
                    {
                        dirDeviceFilesAndSymlinks[dirPath + linkTarget].push_back(fileName);
                    }
                }
                else
                {
                    dirDeviceFilesAndSymlinks[fileName];
                }
            }
        }

        for(auto& deviceFileAndSymlinks : dirDeviceFilesAndSymlinks)
        {
            Gecon::V4l2DeviceFile deviceFile;
            deviceFile.file = deviceFileAndSymlinks.first;
            deviceFile.symlinks = deviceFileAndSymlinks.second;

            deviceFiles.push_back(deviceFile);
        }
    }

    return deviceFiles;
}

bool Gecon::V4l2CameraDeviceManager::isVideoDevice_(const std::string& fileName) const
{
    return (fileName.substr(fileName.find_last_of('/') + 1, 5) == "video") && (open(fileName.c_str(), O_RDWR) > -1);
}
