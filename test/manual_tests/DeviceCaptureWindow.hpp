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

#ifndef DEVICECAPTUREWINDOW_HPP
#define DEVICECAPTUREWINDOW_HPP

#include <QWidget>
#include <QTimer>
#include <QMetaType>

#include "V4L2VideoDevicePolicy.hpp"

typedef Gecon::V4L2VideoDevicePolicy<Gecon::Image> DevicePolicy;

Q_DECLARE_METATYPE(DevicePolicy::DeviceAdapter)

namespace Ui {
    class DeviceCaptureWindow;
}

class DeviceCaptureWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit DeviceCaptureWindow(QWidget *parent = 0);
    ~DeviceCaptureWindow();

public slots:
    void setDevice(int index);
    void startCapture();
    void stopCapture();
    void showImage();

private:
    Ui::DeviceCaptureWindow* ui_;
    QTimer* timer_;

    DevicePolicy::DeviceAdapter device_;
    DevicePolicy::DeviceAdapterList devices_;
};

#endif // DEVICECAPTUREWINDOW_HPP
