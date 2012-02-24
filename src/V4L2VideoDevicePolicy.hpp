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

#ifndef GECON_V4L2VIDEODEVICEPOLICY_HPP
#define GECON_V4L2VIDEODEVICEPOLICY_HPP

#include "V4L2VideoDeviceAdapter.hpp"

#include <set>
#include <list>
#include <memory>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace Gecon
{
    template< typename SnapshotType >
    class V4L2VideoDevicePolicy
    {
    public:
        typedef SnapshotType Snapshot;
        typedef V4L2VideoDeviceAdapter<Snapshot> DeviceAdapter;
        typedef std::list<DeviceAdapter> DeviceAdapterList;

        const DeviceAdapterList& getAvailableDevices();

    private:
        std::set<fs::path> getDeviceFiles_() const;
        bool isVideoDevice_(fs::path filepath) const;

        DeviceAdapterList devices_;
    };
} // namespace Gecon

#include "V4L2VideoDevicePolicy.tpp"

#endif // GECON_V4L2VIDEODEVICEPOLICY_HPP
