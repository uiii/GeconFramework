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

#ifndef GECON_COLOROBJECT_HPP
#define GECON_COLOROBJECT_HPP

#include "Color.hpp"

namespace Gecon
{
    template< typename ColorSpace >
    class ColorObject
    {
    public:
        typedef Gecon::Color<ColorSpace> Color;

        ColorObject(Color color = Color());

        Color color();

    private:
        Color color_;
    };
} // namespace Gecon

#include "ColorObject.tpp"

#endif // GECON_COLOROBJECT_HPP
