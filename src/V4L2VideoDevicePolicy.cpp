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

#include "V4L2VideoDevicePolicy.hpp"

#include <set>

#ifndef VIDEO_DEVICE_DIRS
    #define VIDEO_DEVICE_DIRS { "/dev/", "/dev/v4l/" }
#endif

namespace Gecon
{
    const V4L2VideoDevicePolicy::DeviceAdapterList& V4L2VideoDevicePolicy::getAvailableDevices()
    {
        std::set<fs::path> deviceFiles = getDeviceFiles_();

        DeviceAdapterList::iterator it = devices_.begin();

        for(const fs::path& deviceFile : deviceFiles)
        {
            if(it == devices_.end() || deviceFile < it->file()) // new device
            {
                try
                {
                    devices_.insert(it, DeviceAdapter(deviceFile));
                }
                catch(const std::exception& e)
                {
                    // TODO log error
                }
            }
            else if(deviceFile > it->file()) // device in 'it' is no longer present
            {
                it = devices_.erase(it);
            }
            else // device in 'it' is still present
            {
                ++it;
            }
        }

        return devices_;
    }

    std::set<fs::path> V4L2VideoDevicePolicy::getDeviceFiles_() const
    {
        std::set<fs::path> deviceDirs = VIDEO_DEVICE_DIRS;

        std::set<fs::path> files;
        std::set<fs::path> symlinks;

        for(const fs::path& dir : deviceDirs)
        {
            if(fs::is_directory(dir))
            {
                try
                {
                    fs::directory_iterator it;
                    for(it = fs::directory_iterator(dir); it != fs::directory_iterator(); ++it)
                    {
                        fs::path filepath = it->path();

                        if(isVideoDevice_(filepath))
                        {
                            if(fs::is_symlink(filepath))
                            {
                                symlinks.insert(filepath);
                            }
                            else
                            {
                                files.insert(filepath);
                            }
                        }
                    }
                }
                catch(const fs::filesystem_error& e)
                {
                    // TODO error
                    continue;
                }
            }
        }

        for(const fs::path& symlink : symlinks)
        {
            try
            {
                fs::path filepath = fs::read_symlink(symlink);

                if(files.find(filepath) == files.end()) // filepath not found
                {
                    files.insert(symlink);
                }
            }
            catch(const fs::filesystem_error& e)
            {
                // TODO error
                continue;
            }
        }

        return files;
    }

    bool V4L2VideoDevicePolicy::isVideoDevice_(fs::path filepath) const
    {
        std::string filename = filepath.filename().string();
        return filename.substr(0, 5) == "video";
    }
} // namespace Gecon
