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

#ifndef GECON_V4L2CAMERADEVICE_HPP
#define GECON_V4L2CAMERADEVICE_HPP

#include <string>

#include "common.h"

#include "CameraDevice.hpp"

namespace Gecon
{
    struct V4l2DeviceFile
    {
        std::string file;
        StringList symlinks;
    };

    class V4l2CameraDevice : public CameraDevice
    {
    public:
        V4l2CameraDevice(const V4l2DeviceFile& deviceFile);
        virtual ~V4l2CameraDevice();

        void updateDeviceFile(const V4l2DeviceFile& deviceFile);

        const std::string& name() const;

        void startCapture();
        void stopCapture();

    private:
        V4l2DeviceFile deviceFile_;
    };

    typedef std::shared_ptr<V4l2CameraDevice> V4l2CameraDevicePtr;
    typedef std::shared_ptr<const V4l2CameraDevice> V4l2CameraDeviceConstPtr;
    typedef std::list<V4l2CameraDevicePtr> V4l2CameraDevicePtrList;
}

#endif // GECON_V4L2CAMERADEVICE_HPP
