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

#ifndef GECON_V4L2VIDEODEVICEADAPTER_HPP
#define GECON_V4L2VIDEODEVICEADAPTER_HPP

#include <memory>

#include <boost/filesystem.hpp>

#include "V4L2VideoDeviceCapture.hpp"

namespace fs = boost::filesystem;

namespace Gecon
{
    template< typename Snapshot >
    class V4L2VideoDeviceAdapter
    {
    public:
        V4L2VideoDeviceAdapter();
        V4L2VideoDeviceAdapter(const V4L2VideoDeviceAdapter& another);

        V4L2VideoDeviceAdapter(const fs::path& file);

        ~V4L2VideoDeviceAdapter();

        std::string name() const;

        fs::path file() const;

        /**
         * Open a prepare device for use.
         *
         * Upon successful completion it's guaranteed
         * the device is ready to use.
         * Otherwise an exception will be thrown.
         *
         * @throws std::logic_error
         *     When this does not represent any physical device.
         */
        void open();

        /**
         * Close device.
         *
         * Every opened device should be closed
         * when is not further needed.
         */
        void close();

        /**
         * Get device snapshot
         *
         * @throws std::logic_error
         *     When the device is not opened.
         *
         * @throws std::v4l2_device_error
         *     When capture is not started.
         *     When capture crashed.
         */
        Snapshot getSnapshot();

        bool operator==(const V4L2VideoDeviceAdapter& another) const;
        bool operator!=(const V4L2VideoDeviceAdapter& another) const;

    private:
        bool isOpened_;

        std::shared_ptr<V4L2VideoDeviceCapture> capture_;
    };
} // namespace Gecon

#include "private/V4L2VideoDeviceAdapter.tpp"

#endif // GECON_V4L2VIDEODEVICEADAPTER_HPP
