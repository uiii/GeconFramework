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

#include "Image.hpp"

#include <stdexcept>
#include <iostream>

namespace Gecon
{
    Image::Image():
        width_(0),
        height_(0),
        data_(std::make_shared<ImageRawData>())
    {
    }

    Image::Image(int width, int height):
        width_(width),
        height_(height),
        data_(std::make_shared<ImageRawData>(width * height * COMP_PER_PIXEL, 0))
    {
        if(width < 0 || height < 0)
        {
            throw std::length_error("Image size cannot be negative.");
        }
    }

    std::size_t Image::width() const
    {
        return width_;
    }

    std::size_t Image::height() const
    {
        return height_;
    }

    Color Image::at(int x, int y) const
    {
        checkRange_(x, y);

        const unsigned char* rawPixel = &data_->at((y * width_ + x) * COMP_PER_PIXEL);

        return Color(
            rawPixel[RED],
            rawPixel[GREEN],
            rawPixel[BLUE]
        );
    }

    void Image::setAt(int x, int y, const Color& color)
    {
        checkRange_(x, y);

        makeOwnCopyOfData_();

        unsigned char* rawPixel = &data_->at((y * width_ + x) * COMP_PER_PIXEL);
        //auto rawPixel = data_->at((y * width + x) * COMP_PER_PIXEL);

        rawPixel[RED] = color.red();
        rawPixel[GREEN] = color.green();
        rawPixel[BLUE] = color.blue();
    }

    const ImageRawData& Image::rawData() const
    {
        return *data_;
    }

    void Image::makeOwnCopyOfData_()
    {
        if(data_.use_count() > 1)
        {
            data_ = std::make_shared<ImageRawData>(*data_);
        }
    }

    void Image::checkRange_(int x, int y) const
    {
        if(x < 0 || y < 0 || x > (width_ - 1) || y > (height_ - 1))
        {
            throw std::out_of_range("Reqested position is out of image size.");
        }
    }
}
