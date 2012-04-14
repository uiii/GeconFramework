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

#ifndef GECON_IMAGE_HPP
#define GECON_IMAGE_HPP

#include <vector>
#include <memory>

#include "Color.hpp"

namespace Gecon
{
    template< typename ColorSpaceType >
    class Image
    {
        typedef ColorSpaceType ColorSpace;
        typedef Gecon::Color<ColorSpace> Color;
        typedef std::vector<unsigned char> RawData;

    public:
        Image();
        Image(int width, int height);
        Image(int width, int height, const unsigned char* rawData, std::size_t dataLength);

        void reserve(int width, int height);

        std::size_t width() const;
        std::size_t height() const;

        Color at(int x, int y) const;
        void setAt(int x, int y, const Color& color);

        const RawData& rawData() const;

    private:
        void makeOwnCopyOfData_();
        void checkRange_(int x, int y) const;

        int width_;
        int height_;

        std::shared_ptr<RawData> data_;
    };
}

#include "private/Image.tpp"

#endif // GECON_IMAGE_HPP
