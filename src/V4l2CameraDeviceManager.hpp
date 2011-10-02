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

#ifndef GECON_V4L2CAMERADEVICEMANAGER_HPP
#define GECON_V4L2CAMERADEVICEMANAGER_HPP

#include <map>

#include "common.h"

#include "CameraDeviceManager.hpp"
#include "V4l2CameraDevice.hpp"

namespace Gecon
{
    typedef std::list<V4l2DeviceFile> V4l2DeviceFileList;
    typedef std::map<std::string, V4l2CameraDevicePtr> V4l2CameraDevicePtrMap;

    class V4l2CameraDeviceManager : public CameraDeviceManager
    {
    public:
        V4l2CameraDeviceManager();

        CameraDevicePtrList getAvailableCameraDevices();

    private:
        bool isVideoDevice_(const std::string& fileName) const;
        V4l2DeviceFileList listDeviceFiles_() const;

        V4l2CameraDevicePtrMap devices_;
        CameraDevicePtrList unavailableDevices_;
    };
}

#endif // GECON_V4L2CAMERADEVICEMANAGER_HPP
