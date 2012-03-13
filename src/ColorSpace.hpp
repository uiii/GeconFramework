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

#ifndef GECON_COLORSPACE_HPP
#define GECON_COLORSPACE_HPP

#include <cstring>

namespace Gecon
{
    struct RGB
    {
        static const std::size_t bytesPerPixel = 3;

        typedef unsigned char Component;

        RGB(): r(0), g(0), b(0) {}
        RGB(Component r, Component g, Component b): r(r), g(g), b(b) {}
        RGB(const unsigned char* bytes): r(bytes[0]), g(bytes[1]), b(bytes[2]) {}

        void fillBytes(unsigned char* bytes) const
        {
            bytes[0] = r;
            bytes[1] = g;
            bytes[2] = b;
        }

        bool operator==(const RGB& another)
        {
            return r == another.r && g == another.g && b == another.b;
        }

        bool operator!=(const RGB& another)
        {
            return ! (*this == another);
        }

        Component r;
        Component g;
        Component b;
    };

    struct YCbCr
    {
        static const std::size_t bytesPerPixel = 3;

        typedef unsigned char Component;

        YCbCr(): y(0), cb(128), cr(128) {}
        YCbCr(Component y, Component cb, Component cr): y(y), cb(cb), cr(cr) {}
        YCbCr(const unsigned char* bytes): y(bytes[0]), cb(bytes[1]), cr(bytes[2]) {}

        void fillBytes(unsigned char* bytes) const
        {
            bytes[0] = y;
            bytes[1] = cb;
            bytes[2] = cr;
        }

        bool operator==(const YCbCr& another)
        {
            return y == another.y && cb == another.cb && cr == another.cr;
        }

        bool operator!=(const YCbCr& another)
        {
            return ! (*this == another);
        }

        Component y;
        Component cb;
        Component cr;
    };

    template< typename ToColorSpace, typename FromColorSpace >
    inline ToColorSpace convert(const FromColorSpace& colorSpace)
    {
        return colorSpace;
    }

    template<>
    inline YCbCr convert<YCbCr, RGB>(const RGB& colorSpace)
    {
        unsigned char y = 0.299 * colorSpace.r + 0.587 * colorSpace.g + 0.114 * colorSpace.b;
        unsigned char cb = -0.169 * colorSpace.r + -0.331 * colorSpace.g + 0.5 * colorSpace.b + 128;
        unsigned char cr = 0.5 * colorSpace.r + -0.419 * colorSpace.g + -0.081 * colorSpace.b + 128;

        return YCbCr{y, cb, cr};
    }

    template<>
    inline RGB convert<RGB, YCbCr>(const YCbCr& colorSpace)
    {
        int cr = colorSpace.cr - 128;
        int cb = colorSpace.cb - 128;

        int r = colorSpace.y + 1.4 * cr;
        int g = colorSpace.y + -0.343 * cb + -0.711 * cr;
        int b = colorSpace.y + 1.765 * cb;

        if(r < 0) r = 0;
        if(r > 255) r = 255;

        if(g < 0) g = 0;
        if(g > 255) g = 255;

        if(b < 0) b = 0;
        if(b > 255) b = 255;

        return RGB{(unsigned char)r, (unsigned char)g, (unsigned char)b};
    }
} // namespace Gecon

#endif // GECON_COLORSPACE_HPP
