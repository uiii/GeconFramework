/**
 * This file is part of the Gecon Framework project (https://github.com/uiii/GeconFramework).
 *
 * Copyright (C) 2012 by Richard Jedlička <uiii.dev@gmail.com>
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

#ifndef GECON_COLOROBJECT_HPP
#define GECON_COLOROBJECT_HPP

#include "Color.hpp"
#include "ColorArea.hpp"
#include "Point.hpp"
#include "Fraction.hpp"

#include <list>
#include <chrono>

namespace Gecon
{
    /**
     * Color object reprezentation.
     */
    template< typename ColorSpace >
    class ColorObject
    {
    public:
        typedef Gecon::Color<ColorSpace> Color;

        struct BoundingBox
        {
            BoundingBox(): position(0, 0), width(0), height(0), angle(0) {}

            Point position;

            double width;
            double height;

            double angle;
        };

        typedef std::list<Point> ConvexHull;
        typedef std::list<Point> Border;

        typedef std::chrono::system_clock::time_point Time;

        /**
         * Construct object from color.
         */
        ColorObject(Color color = Color());

        /**
         * Get object's color.
         */
        Color color() const;

        /**
         * Set object's color.
         */
        void setColor(Color color);

        /**
         * Update object's state acording to
         * recognised color area in the snapshot.
         *
         * @param area
         *     Recognised area
         *
         * @param maxPosition
         *     It is similar to snapshot size.
         *     It is used to compute relative position of image.
         */
        void update(ColorArea<ColorSpace>* area, Point maxPosition);

        /**
         * Time of last update.
         */
        Time updateTime();

        bool isVisible() const;
        void setVisible(bool visible);

        /**
         * Get object's position relative to the snapshot size.
         */
        Point position() const;
        
        /**
         * Get object's absolute position in the snapshot.
         */
        Point absolutePosition() const;

        /**
         * Get object's maximum position in the snapshot.
         * It is the same as snapshot size.
         */
        Point maxPosition() const;

        /**
         * Angle of object's rotation.
         */
        int angle() const;

        /**
         * Area of object's bounding box.
         */
        Fraction areaSize() const;

        /**
         * Aspect ration of object's bounding box sides.
         */
        double sidesAspectRatio() const;

        const Border& border() const;
        const BoundingBox& boundingBox() const;
        const ConvexHull& convexHull() const;

    private:
        /**
         * Computes object's convex hull.
         */
        void updateConvexHull_(ColorArea<ColorSpace> *area);

        /**
         * Computes objects's bounding box from object's convex hull.
         */
        void updateMinimalBoundingBox_(const ConvexHull& convexHull);

        Color color_;

        Time updateTime_;
        Point maxPosition_;

        bool isVisible_;

        Border border_;
        ConvexHull convexHull_;
        BoundingBox boundingBox_;
    };
} // namespace Gecon

#include "private/ColorObject.tpp"

#endif // GECON_COLOROBJECT_HPP
