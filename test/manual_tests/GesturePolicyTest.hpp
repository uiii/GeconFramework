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

#ifndef GESTUREPOLICYTEST_HPP
#define GESTUREPOLICYTEST_HPP

#include "ManualTest.hpp"

#include "ColorObjectPolicy.hpp"
#include "ObjectGesturePolicy.hpp"

#include "ImageProcessUI.hpp"
#include "YCbCrColorPicker.hpp"

#include <QMouseEvent>
#include <QPainter>

class GestureRecognitionUI : public ImageProcessUI
{
    Q_OBJECT

    typedef Gecon::ColorObjectPolicy::Objects Objects;
    typedef Gecon::ColorObjectPolicy::Object Object;
    typedef Gecon::ColorObjectPolicy::Object* ObjectPtr;
    typedef Gecon::ColorObjectPolicy::Color Color;

public:
    GestureRecognitionUI()
    {
        connect(getDisplay(0), SIGNAL(clicked(QMouseEvent*)), this, SLOT(imageClicked(QMouseEvent*)));
    }

    virtual ~GestureRecognitionUI()
    {
        clearObjects_();
    }

public slots:
    virtual void setColor(YCbCrColor color)
    {
        clearObjects_();

        objects_.insert(new Object(color));
        cor_.prepareObjects(objects_);
    }

    virtual void imageClicked(QMouseEvent* e)
    {
        Color color = raw_.at(e->pos().x(), e->pos().y());

        setColor(color);
    }

    virtual void processImage()
    {
        raw_ = getImage();

        Objects objects = cor_.recognizeObjects(raw_);

        original_ = toQImage(raw_);
        segmented_ = toQImage(cor_.segmentedImage());

        QPainter painter(&segmented_);

        for(ObjectPtr object : objects)
        {
            const Object::BoundingBox& boundingBox = object->boundingBox();

            const Object::ConvexHull& convexHull = object->convexHull();

            QPolygon convexHullPolygon;
            for(const Gecon::Point& point : convexHull)
            {
                convexHullPolygon << QPoint(point.x, point.y);
            }

            painter.save();
            painter.setPen(Qt::green);
            painter.drawPolygon(convexHullPolygon);
            painter.restore();

            painter.save();
            painter.setPen(Qt::red);

            painter.translate(boundingBox.position.x, boundingBox.position.y);
            painter.rotate(-(boundingBox.angle));
            painter.translate(-boundingBox.width / 2.0, -boundingBox.height / 2.0);

            painter.drawRect(0, 0, boundingBox.width, boundingBox.height);

            painter.restore();
        }

        showImage(original_, 0);
        showImage(segmented_, 1);
    }

private:
    void clearObjects_()
    {
        for(ObjectPtr object : objects_)
        {
            delete object;
        }

        objects_.clear();
    }

    Gecon::ColorObjectPolicy cor_;

    Image raw_;
    QImage original_;
    QImage segmented_;

    Objects objects_;
};

class GesturePolicyTest : public ManualTestSuite
{
public:
    GesturePolicyTest():
        ManualTestSuite("Gesture Policy")
    {
        addTest("Gesture Recognition", std::bind(&GesturePolicyTest::colorObjectRecognition, this));
    }

    void colorObjectRecognition()
    {
        QApplication app(ManualTester::argc, ManualTester::argv);

        GestureRecognitionUI ui;
        ui.show();

        app.exec();
    }
};

#endif // GESTUREPOLICYTEST_HPP
