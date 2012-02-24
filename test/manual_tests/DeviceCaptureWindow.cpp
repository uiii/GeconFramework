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

#include "DeviceCaptureWindow.hpp"
#include "ui_DeviceCaptureWindow.h"

#include <QPixmap>
#include <QVariant>

DeviceCaptureWindow::DeviceCaptureWindow(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::DeviceCaptureWindow)
{
    ui_->setupUi(this);

    devices_ = DevicePolicy().getAvailableDevices();

    for(auto device : devices_)
    {
        ui_->deviceList->addItem(device.name().c_str(), QVariant::fromValue(device));
    }

    if(! devices_.empty())
    {
        device_ = devices_.front();
    }

    timer_ = new QTimer();
    timer_->setInterval(30);

    connect(ui_->deviceList, SIGNAL(currentIndexChanged(int)), this, SLOT(setDevice(int)));
    connect(ui_->openButton, SIGNAL(clicked()), this, SLOT(startCapture()));
    connect(ui_->closeButton, SIGNAL(clicked()), this, SLOT(stopCapture()));
    connect(timer_, SIGNAL(timeout()), this, SLOT(showImage()));
}

DeviceCaptureWindow::~DeviceCaptureWindow()
{
    delete ui_;
}

void DeviceCaptureWindow::setDevice(int index)
{
    device_ = ui_->deviceList->itemData(index).value<DevicePolicy::DeviceAdapter>();
}

void DeviceCaptureWindow::startCapture()
{
    device_.open();
    timer_->start();
}

void DeviceCaptureWindow::stopCapture()
{
    timer_->stop();
    device_.close();
}

void DeviceCaptureWindow::showImage()
{
    DevicePolicy::Snapshot img = device_.getSnapshot();

    ui_->image->setPixmap(QPixmap::fromImage(QImage((const uchar*)&(img.rawData()[0]), img.width(), img.height(), img.width() * 3, QImage::Format_RGB888)));
}
