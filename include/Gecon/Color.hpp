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

#ifndef GECON_COLOR_HPP
#define GECON_COLOR_HPP

#include <iostream>

#include "ColorSpace.hpp"

namespace Gecon
{
    template< typename ColorSpace >
    class Color : public ColorSpace
    {
    public:
        Color(ColorSpace colorSpace = ColorSpace()):
            ColorSpace(colorSpace)
        {}

        template< typename AnotherColorSpace >
        Color(const Color<AnotherColorSpace>& color);
    };

    template< typename ToColorSpace, typename FromColorSpace >
    inline Color<ToColorSpace> convert(const Color<FromColorSpace>& color)
    {
        return convert<ToColorSpace>(static_cast<const FromColorSpace&>(color));
    }

    template< typename ColorSpace >
    template< typename AnotherColorSpace >
    inline Color<ColorSpace>::Color(const Color<AnotherColorSpace>& color)
    {
        *this = convert<ColorSpace>(color);
    }
}

#endif // GECON_COLOR_HPP
