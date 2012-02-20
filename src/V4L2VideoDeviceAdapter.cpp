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

#include "V4L2VideoDeviceAdapter.hpp"

namespace Gecon
{
    V4L2VideoDeviceAdapter::V4L2VideoDeviceAdapter():
        isOpened_(false),
        capture_(0)
    {
    }

    V4L2VideoDeviceAdapter::V4L2VideoDeviceAdapter(const fs::path& file):
        isOpened_(false),
        capture_(std::make_shared<V4L2VideoDeviceCapture>(file))
    {
    }

    V4L2VideoDeviceAdapter::V4L2VideoDeviceAdapter(const V4L2VideoDeviceAdapter& another):
        isOpened_(false),
        capture_(another.capture_)
    {
    }

    V4L2VideoDeviceAdapter::~V4L2VideoDeviceAdapter()
    {
        close();
    }

    std::string V4L2VideoDeviceAdapter::name() const
    {
        // TODO - ziskat udaj o nazvu ze zarizeni
        return file().string();
    }

    fs::path V4L2VideoDeviceAdapter::file() const
    {
        if(! capture_)
        {
            return fs::path();
        }

        return capture_->file();
    }

    /**
     * Open a prepare device for use.
     *
     * Upon successful completion it's guaranteed
     * the device is ready to use.
     * Otherwise an exception will be thrown.
     *
     * @throws // TODO
     */
    void V4L2VideoDeviceAdapter::open()
    {
        if(! capture_)
        {
            return;
        }

        if(! isOpened_)
        {
            capture_->start();

            isOpened_ = true;
        }
    }

    /**
     * Close device.
     *
     * Any opened device should be closed
     * when is not further needed.
     */
    void V4L2VideoDeviceAdapter::close()
    {
        if(! capture_)
        {
            return;
        }

        if(isOpened_)
        {
            capture_->stop();

            isOpened_ = false;
        }
    }

    V4L2VideoDeviceAdapter::Snapshot V4L2VideoDeviceAdapter::getSnapshot()
    {
        if(! capture_)
        {
            return Snapshot();
        }

        try
        {
            return capture_->getSnapshot();
        }
        catch(const v4l2_device_error& e)
        {
            isOpened_ = false;
            throw;
        }
    }
} // namespace Gecon
