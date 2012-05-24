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
    /**
     * Represents rast image
     *
     * Copy of one image shares the same data
     * until one copy wants to change the data.
     * In that time the image copy makes own copy of data.
     *
     * @tparam ColorSpaceType
     *     Specifies the way of storing color of one pixel.
     *     
     */
    template< typename ColorSpaceType >
    class Image
    {
        typedef ColorSpaceType ColorSpace;
        typedef Gecon::Color<ColorSpace> Color;
        typedef std::vector<unsigned char> RawData;

    public:
        Image();

        /**
         * Construct empty image of specified size.
         */
        Image(int width, int height);

        /**
         * Construct image of specified size initialized by pixel data.
         *
         * @param rawData
         *     Array of pixel data. Must correspond to image color space.
         *
         * @param dataLength
         *     Length of the data array.
         *     Must be equal to width * height * bytes per pixel specified by color space.
         */
        Image(int width, int height, const unsigned char* rawData, std::size_t dataLength);

        /**
         * Reserve space to be able to store widt * height pixels.
         * New created space is initializes by zeros.
         */
        void reserve(int width, int height);

        std::size_t width() const;
        std::size_t height() const;

        /**
         * Get color of pixel on specified position in the image.
         */
        Color at(int x, int y) const;

        /**
         * Set color of pixel on specified position in the image.
         */
        void setAt(int x, int y, const Color& color);

        /**
         * Get raw data of image.
         * Raw data means the exact data where image stores imformations about pixels.
         */
        const RawData& rawData() const;

    private:
        /**
         * Make of copy of data if the data are
         * shared among more images.
         * Otherwise nothing will happen.
         */
        void makeOwnCopyOfData_();

        /**
         * Check if the desired position
         * isn't out of the image.
         */
        void checkRange_(int x, int y) const;

        int width_;
        int height_;

        std::shared_ptr<RawData> data_;
    };
}

#include "private/Image.tpp"

#endif // GECON_IMAGE_HPP
