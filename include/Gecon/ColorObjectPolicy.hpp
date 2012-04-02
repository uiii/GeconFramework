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
#include "Image.hpp"

namespace Gecon
{
    class ColorObjectPolicy
    {
    public:
        typedef YCbCr ColorSpace;
        typedef Gecon::Color<ColorSpace> Color;

        typedef ColorObject<ColorSpace> Object;
        typedef Object* ObjectPtr;
        typedef std::set<ObjectPtr> ObjectSet;
        typedef std::vector<ObjectPtr> ObjectVector;

        typedef Gecon::ColorArea<ColorSpace> Area;
        typedef Area* AreaPtr;
        typedef std::list<AreaPtr> AreaList;

        typedef Area::Block AreaBlock;
        typedef Area::BlockList AreaBlockList;

        typedef boost::dynamic_bitset<> Bitset;
        typedef std::vector<Bitset> ColorMap;

        typedef Gecon::Image<RGB> OutputImage;
        typedef Gecon::Color<RGB> OutputImageColor;

        ColorObjectPolicy();

        void prepareObjects(const ObjectSet& definedObjects);

        template< typename Snapshot >
        ObjectSet recognizeObjects(const Snapshot& snapshot);

        OutputImage image();

    private:
        template< typename Snapshot >
        void createBlocks_(Snapshot snapshot, std::size_t row, AreaBlockList &currentRowBlocks);

        void connectBlocks_(const AreaBlockList& lastRow, AreaBlockList& currentRow);
        AreaPtr createArea_(const AreaBlock& block);

        void selectVisibleObjects_(ObjectSet &visibleObjects);

        ObjectVector objects_;

        ColorMap yMap_;
        ColorMap cbMap_;
        ColorMap crMap_;

        AreaList areas_;

        OutputImage image_;
    };
} // namespace Gecon

#include "private/ColorObjectPolicy.tpp"

#endif // GECON_COLOROBJECTPOLICY_HPP
