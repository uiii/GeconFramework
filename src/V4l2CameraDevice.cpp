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

#include "V4l2CameraDevice.hpp"

Gecon::V4l2CameraDevice::V4l2CameraDevice(const Gecon::V4l2DeviceFile& deviceFile):
    deviceFile_(deviceFile)
{
}

Gecon::V4l2CameraDevice::~V4l2CameraDevice()
{
}

void Gecon::V4l2CameraDevice::startCapture()
{
    // TODO
}

void Gecon::V4l2CameraDevice::stopCapture()
{
    // TODO
}

const std::string & Gecon::V4l2CameraDevice::name() const
{
    return deviceFile_.file; // TODO ziskat nazev kamery
}

void Gecon::V4l2CameraDevice::updateDeviceFile(const Gecon::V4l2DeviceFile &deviceFile)
{
    if(deviceFile_.file == deviceFile.file)
    {
        deviceFile_.symlinks = deviceFile.symlinks;
    }

    // TODO nevim jestli je to rozumny
}
