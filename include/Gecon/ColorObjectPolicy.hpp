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

#ifndef GECON_COLOROBJECTPOLICY_HPP
#define GECON_COLOROBJECTPOLICY_HPP

#include <memory>
#include <vector>
#include <list>
#include <set>

#include <cstdint>

#include <boost/dynamic_bitset.hpp>

#include "ColorObject.hpp"
#include "ColorObjectSet.hpp"
#include "Image.hpp"

#include "Point.hpp"

namespace Gecon
{
    class ColorObjectPolicy
    {
    public:
        static config_variable<std::size_t> MINIMAL_OBJECT_SIZE_FRACTION;

        typedef YCbCr ColorSpace;
        typedef Gecon::Color<ColorSpace> Color;

        typedef ColorObject<ColorSpace> Object;
        typedef ColorObjectSet<ColorSpace> Objects;

        typedef Gecon::ColorArea<ColorSpace> Area;
        typedef std::list<Area*> Areas;

        typedef Area::Block AreaBlock;
        typedef Area::BlockList AreaBlockList;

        typedef boost::dynamic_bitset<> Bitset;
        typedef std::vector<Bitset> ColorMap;

        typedef Gecon::Image<RGB> OutputImage;
        typedef Gecon::Color<RGB> OutputImageColor;

        ColorObjectPolicy();
        ColorObjectPolicy(const ColorObjectPolicy& another);

        ColorObjectPolicy& operator=(const ColorObjectPolicy& another);

        void prepareObjects(const Objects& definedObjects);

        template< typename Snapshot >
        Objects recognizeObjects(const Snapshot& snapshot);

        const OutputImage& segmentedImage();

    private:
        template< typename Snapshot >
        void createBlocks_(Snapshot snapshot, std::size_t row, AreaBlockList &currentRowBlocks);

        void connectBlocks_(const AreaBlockList& lastRow, AreaBlockList& currentRow);
        Area* createArea_(const AreaBlock& block);

        void selectVisibleObjects_(Objects& visibleObjects, Point snapshotSize);

        Objects objects_;

        Areas areas_;

        OutputImage image_;
    };
} // namespace Gecon

#include "private/ColorObjectPolicy.tpp"

#endif // GECON_COLOROBJECTPOLICY_HPP
