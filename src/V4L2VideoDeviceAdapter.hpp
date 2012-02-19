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

#ifndef GECON_V4L2VIDEODEVICEADAPTER_HPP
#define GECON_V4L2VIDEODEVICEADAPTER_HPP

#include <memory>

#include <boost/filesystem.hpp>

#include "V4L2VideoDeviceCapture.hpp"

namespace fs = boost::filesystem;

namespace Gecon
{
    class V4L2VideoDeviceAdapter
    {
    public:
        typedef V4L2VideoDeviceCapture::Snapshot Snapshot; // TODO

        V4L2VideoDeviceAdapter();
        V4L2VideoDeviceAdapter(const fs::path& file);
        V4L2VideoDeviceAdapter(const V4L2VideoDeviceAdapter& another);
        ~V4L2VideoDeviceAdapter();

        std::string name() const;

        fs::path file() const;

        void open();
        void close();

        Snapshot getSnapshot();

    private:
        bool isOpened_;

        std::shared_ptr<V4L2VideoDeviceCapture> capture_;
    };
} // namespace Gecon

#endif // GECON_V4L2VIDEODEVICEADAPTER_HPP
