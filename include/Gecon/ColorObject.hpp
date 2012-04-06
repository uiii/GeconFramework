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

#include <list>

namespace Gecon
{
    template< typename ColorSpace >
    class ColorObject
    {
    public:
        typedef Gecon::Color<ColorSpace> Color;

        struct Point
        {
            double x;
            double y;
        };

        struct BoundingBox
        {
            BoundingBox(): position({0, 0}), width(0), height(0), angle(0) {}

            Point position;

            double width;
            double height;

            double angle;
        };

        typedef std::list<Point> ConvexHull;
        typedef std::list<Point> Border;

        ColorObject(Color color = Color());

        Color color() const;

        void update(ColorArea<ColorSpace> *area);

        bool isVisible() const;
        void setVisible(bool visible);

        const Border& border() const;
        const BoundingBox& boundingBox() const;
        const ConvexHull& convexHull() const;

    private:
        void updateConvexHull_(ColorArea<ColorSpace> *area);
        void updateMinimalBoundingBox_(const ConvexHull& convexHull);

        Color color_;

        bool isVisible_;

        Border border_;
        ConvexHull convexHull_;
        BoundingBox boundingBox_;
    };
} // namespace Gecon

#include "private/ColorObject.tpp"

#endif // GECON_COLOROBJECT_HPP
