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

#include "../Image.hpp"

#include <stdexcept>
#include <iostream>

namespace Gecon
{
    template< typename ColorSpace >
    Image<ColorSpace>::Image():
        width_(0),
        height_(0),
        data_(std::make_shared<RawData>())
    {
    }

    template< typename ColorSpace >
    Image<ColorSpace>::Image(int width, int height):
        width_(width),
        height_(height),
        data_(std::make_shared<RawData>(width * height * ColorSpace::bytesPerPixel, 0))
    {
        if(width < 0 || height < 0)
        {
            throw std::length_error("Image size cannot be negative.");
        }
    }

    template< typename ColorSpace >
    Image<ColorSpace>::Image(int width, int height, const unsigned char* rawData, std::size_t dataLength):
        width_(width),
        height_(height),
        data_(std::make_shared<RawData>(rawData, rawData + dataLength))
    {
        if(width * height * ColorSpace::bytesPerPixel != dataLength)
        {
            throw std::length_error("Image size doesn't correspond to raw data length");
        }
    }

    template< typename ColorSpace >
    std::size_t Image<ColorSpace>::width() const
    {
        return width_;
    }

    template< typename ColorSpace >
    std::size_t Image<ColorSpace>::height() const
    {
        return height_;
    }

    template< typename ColorSpace >
    typename Image<ColorSpace>::Color Image<ColorSpace>::at(int x, int y) const
    {
        checkRange_(x, y);

        const unsigned char* rawPixel = &data_->at((y * width_ + x) * ColorSpace::bytesPerPixel);

        return Color(rawPixel);
    }

    template< typename ColorSpace >
    void Image<ColorSpace>::setAt(int x, int y, const typename Image<ColorSpace>::Color& color)
    {
        checkRange_(x, y);

        makeOwnCopyOfData_();

        unsigned char* rawPixel = &data_->at((y * width_ + x) * ColorSpace::bytesPerPixel);

        color.fillBytes(rawPixel);
    }

    template< typename ColorSpace >
    const typename Image<ColorSpace>::RawData& Image<ColorSpace>::rawData() const
    {
        return *data_;
    }

    template< typename ColorSpace >
    void Image<ColorSpace>::makeOwnCopyOfData_()
    {
        if(data_.use_count() > 1)
        {
            data_ = std::make_shared<RawData>(*data_);
        }
    }

    template< typename ColorSpace >
    void Image<ColorSpace>::checkRange_(int x, int y) const
    {
        if(x < 0 || y < 0 || x > (width_ - 1) || y > (height_ - 1))
        {
            throw std::out_of_range("Reqested position is out of image size.");
        }
    }
}
