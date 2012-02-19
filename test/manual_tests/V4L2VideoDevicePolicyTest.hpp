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
#ifndef V4L2VIDEODEVICEPOLICYTEST_HPP
#define V4L2VIDEODEVICEPOLICYTEST_HPP

#include <iostream>

#include <QApplication>

#include "ManualTestSuite.hpp"
#include "ManualTester.hpp"

#include "V4L2VideoDevicePolicy.hpp"

#include "DeviceCaptureWindow.hpp"

class V4L2VideoDevicePolicyTest : public ManualTestSuite
{
public:
    V4L2VideoDevicePolicyTest():
        ManualTestSuite("V4L2 Device policy")
    {
        addTest("Device List", boost::bind(&V4L2VideoDevicePolicyTest::deviceListTest, this));
        addTest("Device Capture", boost::bind(&V4L2VideoDevicePolicyTest::deviceCaptureTest, this));
    }

    void deviceListTest()
    {
        Gecon::V4L2VideoDevicePolicy::DeviceAdapterList deviceList = Gecon::V4L2VideoDevicePolicy().getAvailableDevices();

        std::cout << "Available V4L2 devices:" << std::endl;
        for(Gecon::V4L2VideoDevicePolicy::DeviceAdapter& device : deviceList)
        {
            std::cout << device.file() << std::endl;
        }
    }

    void deviceCaptureTest()
    {
        QApplication app(ManualTester::argc, ManualTester::argv);

        DeviceCaptureWindow window;
        window.show();

        app.exec();
    }
};

#endif //V4L2VIDEODEVICEPOLICYTEST_HPP
