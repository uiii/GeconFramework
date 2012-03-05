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

#ifndef COLOROBJECTPOLICYTEST_HPP
#define COLOROBJECTPOLICYTEST_HPP

#include "ManualTest.hpp"

#include "ColorObjectRecognition.hpp"

#include "ImageProcessUI.hpp"
#include "YCbCrColorPicker.hpp"

#include <QMouseEvent>

class ObjectRecognitionUI : public ImageProcessUI
{
    Q_OBJECT

    typedef Gecon::ColorObjectRecognition::ObjectSet ObjectSet;
    typedef Gecon::ColorObjectRecognition::Object Object;
    typedef Gecon::ColorObjectRecognition::Color Color;

public:
    ObjectRecognitionUI()
    {
        picker_ = new YCbCrColorPicker(this);
        layout()->addWidget(picker_);

        connect(getDisplay(0), SIGNAL(clicked(QMouseEvent*)), this, SLOT(imageClicked(QMouseEvent*)));
    }

    virtual ~ObjectRecognitionUI() {}

    virtual void setColor(Color color)
    {
        ObjectSet objects;
        objects.push_back(new Object(color));
        cor_.prepareObjectsForRecognition(objects);

        picker_->updateColor(color);
    }

public slots:
    virtual void imageClicked(QMouseEvent* e)
    {
        Color color = img_.at(e->pos().x(), e->pos().y());
        setColor(color);
    }

    virtual void processImage()
    {
        img_ = getImage();

        cor_.recognizeObjects(img_);

        showImage(img_, 0);
        showImage(cor_.image(), 1);
    }

private:
    Gecon::ColorObjectRecognition cor_;
    YCbCrColorPicker* picker_;

    Image img_;
};

class ColorObjectPolicyTest : public ManualTestSuite
{
public:
    ColorObjectPolicyTest():
        ManualTestSuite("Color Object Policy")
    {
        addTest("Color Object Recognition", boost::bind(&ColorObjectPolicyTest::colorObjectRecognition, this));
    }

    void colorObjectRecognition()
    {
        QApplication app(ManualTester::argc, ManualTester::argv);

        ObjectRecognitionUI ui;
        ui.show();

        app.exec();
    }
};

#endif // COLOROBJECTPOLICYTEST_HPP
