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
#include <QLabel>

#include "ui_ImageProcessUI.h"

#include "V4L2VideoDevicePolicy.hpp"

typedef Gecon::V4L2VideoDevicePolicy<Gecon::Image<Gecon::RGB>> DevicePolicy;

Q_DECLARE_METATYPE(DevicePolicy::DeviceAdapter)

namespace Ui {
    class ImageProcessUI;
}

class ImageDisplay : public QLabel
{
    Q_OBJECT

public:
    ImageDisplay(QWidget *parent);

signals:
    void clicked(QMouseEvent* ev);

protected:
    void mousePressEvent(QMouseEvent *ev);
};

class ImageProcessUI : public QWidget
{
    Q_OBJECT
    
public:
    typedef DevicePolicy::Snapshot Image;

    explicit ImageProcessUI(QWidget *parent = 0);
    ~ImageProcessUI();

    Image getImage();

    void createDisplay(std::size_t index);
    ImageDisplay* getDisplay(std::size_t index = 0);

    template< typename ImageType >
    void showImage(const ImageType& img, std::size_t displayIndex = 0);

public slots:
    void setDevice(int index);
    void startCapture();
    void stopCapture();

    virtual void processImage();

protected:
    Ui::ImageProcessUI *ui_;

    DevicePolicy::DeviceAdapter device_;
    DevicePolicy::DeviceAdapterList devices_;

    std::vector<ImageDisplay*> displays_;

private:
    QTimer* timer_;
};

template< typename ImageType >
void ImageProcessUI::showImage(const ImageType &img, std::size_t displayIndex)
{
    createDisplay(displayIndex);

    ui_->displayArea->setVisible(true);

    ImageDisplay* display = displays_.at(displayIndex);
    display->setVisible(true);
    display->setPixmap(QPixmap::fromImage(QImage((const uchar*)&(img.rawData()[0]), img.width(), img.height(), img.width() * 3, QImage::Format_RGB888)));
}


#endif // IMAGEPROCESSUI_HPP
