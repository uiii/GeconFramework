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

#ifndef GECON_DEVICELIST_HPP
#define GECON_DEVICELIST_HPP

#include <memory>
#include <list>
#include <stdexcept>

namespace Gecon
{
    /**
     * DeviceManager manages available devices.
     *
     * Available devices are obtained according to DevicePolicy
     *
     * @tparam DevicePolicy
     *     Policy class describing handling with devices. // TODO
     */
    template< typename DevicePolicy >
    class DeviceManager : public DevicePolicy
    {
    public:
        typedef typename DevicePolicy::DeviceAdapter DeviceAdapter;
        typedef typename DevicePolicy::DeviceAdapterPtr DeviceAdapterPtr;
        typedef typename DevicePolicy::DeviceAdapterList DeviceAdapterList;

        /**
         * Get selected device.
         *
         * If there is no device selected, tries to select
         * the first device from available devices.
         *
         * @returns
         *     Selected device if any.
         *
         * @throws std::logic_error
         *     If there is no device selected and no available devices are present.
         */
        DeviceAdapter& selectedDevice()
        {
            if(! selectedDevice_)
            {
                DeviceAdapterList deviceList = DevicePolicy::listAvailableDevices();

                if(deviceList.empty())
                {
                    throw std::logic_error("Cannot select device: No devices available"); // TODO vlastni vyjimky
                }

                selectDevice(deviceList.first());
            }

            return *selectedDevice_;
        }

        /**
         * Select a device.
         *
         * Only one device can be currently selected per DeviceManager.
         *
         * @param device
         *     Pointer to device to be selected.
         */
        void selectDevice(DeviceAdapterPtr device)
        {
            if(selectedDevice_ != device)
            {
                selectedDevice_ = device;
            }
        }

    private:
        DeviceAdapterPtr selectedDevice_;
    };
} // namespace Gecon

#endif // GECON_DEVICELIST_HPP
