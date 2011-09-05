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

#ifndef GECON_COLOR_H
#define GECON_COLOR_H

namespace Gecon
{
    class Color
    {
    public:
        Color();
        Color(
            unsigned char red,
            unsigned char green,
            unsigned char blue
        );

        unsigned char red() const;
        unsigned char green() const;
        unsigned char blue() const;

        bool operator==(const Color& another) const;
        bool operator!=(const Color& another) const;
        
    private:
        unsigned char red_;
        unsigned char green_;
        unsigned char blue_;
    };
}

#endif
