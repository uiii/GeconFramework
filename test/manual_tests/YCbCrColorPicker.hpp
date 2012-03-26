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

#ifndef YCBCRCOLORPICKER_HPP
#define YCBCRCOLORPICKER_HPP

#include <QWidget>
#include <QLabel>

#include "Color.hpp"

typedef Gecon::Color<Gecon::YCbCr> YCbCrColor;
typedef Gecon::Color<Gecon::RGB> RGBColor;

class LuminancePicker : public QLabel
{
    Q_OBJECT

public:
    LuminancePicker(QWidget* parent = 0);

    void updateColor(YCbCrColor color);

signals:
    void colorChanged(YCbCrColor color);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

private:
    YCbCrColor color_;
};

class ChrominancePicker : public QLabel
{
    Q_OBJECT

public:
    ChrominancePicker(QWidget* parent = 0);

    void updateColor(YCbCrColor color);

signals:
    void colorChanged(YCbCrColor color);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

private:
    YCbCrColor color_;
};

class YCbCrColorPicker : public QWidget
{
    Q_OBJECT

public:
    explicit YCbCrColorPicker(QWidget *parent = 0);
    
signals:
    void colorChanged(YCbCrColor color);

public slots:
    void updateColor(YCbCrColor color);

private:
    LuminancePicker* luminance_;
    ChrominancePicker* chrominance_;
};

#endif // YCBCRCOLORPICKER_HPP
