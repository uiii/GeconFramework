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

#include "Camera.hpp"

Gecon::Camera::Camera(Gecon::CameraDevicePtr cameraDevice):
    cameraDevice_(cameraDevice),
    openingsCount_(std::make_shared<int>(0))
{
}

void Gecon::Camera::open()
{
    // TODO thread safe

    if(*openingsCount_ == 0)
    {
        cameraDevice_->startCapture();
    }

    ++(*openingsCount_);
}

void Gecon::Camera::close()
{
    // TODO thread safe

    if(*openingsCount_ == 1)
    {
        cameraDevice_->stopCapture();
    }

    --(*openingsCount_);
}

bool Gecon::Camera::isOpened() const
{
    // TODO thread safe

    return *openingsCount_ > 0;
}

Gecon::CameraDeviceConstPtr Gecon::Camera::device() const
{
    return cameraDevice_;
}
