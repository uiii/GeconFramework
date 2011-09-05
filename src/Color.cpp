/**
 * This file is part of the Gecon Framework project.
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

#include "Color.hpp"

namespace Gecon
{
    Color::Color():
        red_(0),
        green_(0),
        blue_(0)
    {
    }

    Color::Color(
        unsigned char red,
        unsigned char green,
        unsigned char blue
    ):
        red_(red),
        green_(green),
        blue_(blue)
    {
    }

    unsigned char Color::red() const
    {
        return red_;
    }

    unsigned char Color::green() const
    {
        return green_;
    }

    unsigned char Color::blue() const
    {
        return blue_;
    }

    bool Color::operator==(const Color& another) const
    {
        return red_ == another.red_
            && green_ == another.green_
            && blue_ == another.blue_
        ;
    }

    bool Color::operator!=(const Color& another) const
    {
        return ! (*this == another);
    }
}
