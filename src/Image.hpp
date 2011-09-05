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

#ifndef GECON_IMAGE_H
#define GECON_IMAGE_H

#include <vector>
#include <memory>

#include "Color.hpp"

namespace Gecon
{
    typedef std::vector<Color> ImageData;
    typedef std::vector<unsigned char> ImageRawData;

    class Image
    {
        /* constants determinig the count and
           order of color components in raw data */

        static const std::size_t COMP_PER_PIXEL = 3;
        
        static const std::size_t RED = 2;
        static const std::size_t GREEN = 1;
        static const std::size_t BLUE = 0;

    public:
        Image();
        Image(int width, int height);

        std::size_t width() const;
        std::size_t height() const;

        Color at(int x, int y) const;
        void setAt(int x, int y, const Color& color);

        const ImageRawData& rawData() const;

    private:
        void makeOwnCopyOfData_();
        void checkRange_(int x, int y) const;

        std::size_t width_;
        std::size_t height_;

        std::shared_ptr<ImageRawData> data_;
    };
}

#endif
