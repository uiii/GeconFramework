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

#ifndef IMAGEPROCESSUI_HPP
#define IMAGEPROCESSUI_HPP

#include <QWidget>

#include <QTimer>
#include <QMetaType>

#include "ui_ImageProcessUI.h"

#include "V4L2VideoDevicePolicy.hpp"

typedef Gecon::V4L2VideoDevicePolicy<Gecon::Image<Gecon::RGB>> DevicePolicy;

Q_DECLARE_METATYPE(DevicePolicy::DeviceAdapter)

namespace Ui {
    class ImageProcessUI;
}

class ImageProcessUI : public QWidget
{
    Q_OBJECT
    
public:
    typedef DevicePolicy::Snapshot Image;

    explicit ImageProcessUI(QWidget *parent = 0);
    ~ImageProcessUI();

    Image getImage();
    
public slots:
    void setDevice(int index);
    void startCapture();
    void stopCapture();
    void showImage(const Image& img);
    virtual void processImage();

protected:
    Ui::ImageProcessUI *ui_;

    DevicePolicy::DeviceAdapter device_;
    DevicePolicy::DeviceAdapterList devices_;

private:
    QTimer* timer_;
};

#endif // IMAGEPROCESSUI_HPP
