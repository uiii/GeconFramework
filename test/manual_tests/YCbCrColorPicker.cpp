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

#include "YCbCrColorPicker.hpp"

#include <QImage>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>

#include <iostream>

LuminancePicker::LuminancePicker(QWidget *parent):
    QLabel(parent)
{
    setFixedSize(256,256);
    updateColor(YCbCrColor());
}

void LuminancePicker::updateColor(YCbCrColor color)
{
    color_ = color;
    repaint();
}

void LuminancePicker::paintEvent(QPaintEvent *)
{
    QImage img(256, 256, QImage::Format_RGB32);
    for(std::size_t cr = 0; cr < 256; ++cr)
    {
        for(std::size_t cb = 0; cb < 256; ++cb)
        {
            RGBColor rgb(YCbCrColor({color_.y,(uchar)cb,(uchar)cr}));
            ((uint*)img.scanLine(cr))[cb] = QColor(rgb.r, rgb.g, rgb.b).rgb();
        }
    }

    QPainter painter(this);

    QRect imageRect(0,0,256,256);
    painter.drawPixmap(imageRect, QPixmap::fromImage(img));
    painter.setPen(Qt::red);

    QRect rect(color_.cb - 20, color_.cr - 20, 40, 40);
    painter.drawRect(rect.intersected(imageRect));
}

ChrominancePicker::ChrominancePicker(QWidget *parent):
    QLabel(parent)
{
    setFixedSize(20, 256);
    updateColor(YCbCrColor());
}

void ChrominancePicker::updateColor(YCbCrColor color)
{
    color_ = color;
    repaint();
}

void ChrominancePicker::paintEvent(QPaintEvent *)
{
    QImage img(20, 256, QImage::Format_RGB32);
    for(std::size_t y = 0; y < 256; ++y)
    {
        RGBColor rgb(YCbCrColor({(uchar)(255 - y),color_.cb,color_.cr}));
        for(std::size_t i = 0; i < 20; ++i)
        {
            ((uint*)img.scanLine(y))[i] = QColor(rgb.r, rgb.g, rgb.b).rgb();
        }
    }

    QPainter painter(this);

    QRect imageRect(0,0,256,256);
    painter.drawPixmap(imageRect, QPixmap::fromImage(img));
    painter.setPen(Qt::red);

    QRect rect(0, color_.y - 50, 19, 100);
    painter.drawRect(rect.intersected(imageRect));
}

YCbCrColorPicker::YCbCrColorPicker(QWidget *parent) :
    QWidget(parent)
{
    chrominance_ = new ChrominancePicker(this);
    luminance_ = new LuminancePicker(this);

    QWidget* colorPanel = new QWidget(this);

    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(luminance_);
    hbox->addWidget(chrominance_);

    colorPanel->setLayout(hbox);

    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->addWidget(colorPanel);

    setLayout(vbox);
}

void YCbCrColorPicker::updateColor(YCbCrColor color)
{
    luminance_->updateColor(color);
    chrominance_->updateColor(color);
}
