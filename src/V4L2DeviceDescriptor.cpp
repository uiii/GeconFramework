/**
 * This file is part of the Gecon Framework project (https://github.com/uiii/GeconFramework).
 *
 * Copyright (C) 2012 by Richard Jedlička <uiii.dev@gmail.com>
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

#include "V4L2DeviceDescriptor.hpp"

#include <fcntl.h>
#include <libv4l2.h>

#include "V4L2Error.hpp"

namespace Gecon
{
    V4L2FileDescriptor::V4L2FileDescriptor(int fd):
        fd_(fd)
    {
    }

    V4L2FileDescriptor::~V4L2FileDescriptor()
    {
        v4l2_close(fd_);
    }

    V4L2FileDescriptor::operator int() const
    {
        return fd_;
    }
}

namespace Gecon
{
    V4L2DeviceDescriptor::V4L2DeviceDescriptor()
    {
    }

    V4L2DeviceDescriptor::V4L2DeviceDescriptor(const fs::path& file):
        file_(file)
    {
        int fd = v4l2_open(file.c_str(), O_RDWR | O_NONBLOCK, 0);

        if(fd == -1)
        {
            throw v4l2_device_error(file, "Cannot open device %device%", errno_message());
        }

        fd_ = std::make_shared<V4L2FileDescriptor>(fd);
    }

    V4L2DeviceDescriptor::operator int() const
    {
        if(fd_)
        {
            return (int)(*fd_);
        }
        else
        {
            return -1;
        }
    }

    fs::path V4L2DeviceDescriptor::file() const
    {
        return file_;
    }

    void V4L2DeviceDescriptor::close()
    {
        fd_.reset();
    }
}
