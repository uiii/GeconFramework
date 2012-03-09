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

#ifndef GECON_COLOROBJECTRECOGNITION_HPP
#define GECON_COLOROBJECTRECOGNITION_HPP

#include <set>
#include <vector>
#include <list>

#include <cstdint>

#include <boost/dynamic_bitset.hpp>

#include "ColorObject.hpp"
#include "Image.hpp"
#include "Area.hpp"

namespace Gecon
{
    class ColorObjectRecognition
    {
    public:
        typedef YCbCr ColorSpace;
        typedef Gecon::Color<ColorSpace> Color;

        typedef ColorObject<ColorSpace> Object;
        typedef std::vector<Object*> ObjectSet; // TODO
        typedef std::vector<Object*> ObjectList;

        typedef Gecon::AreaBlock<Object> AreaBlock;
        typedef std::list<AreaBlock> AreaBlockList;

        typedef Gecon::Area<Object> Area;
        typedef std::list<Area*> AreaList;

        typedef boost::dynamic_bitset<> Bitset;
        typedef std::vector<Bitset> ColorMap;

        typedef Gecon::Image<RGB> OutputImage;
        typedef Gecon::Color<RGB> OutputImageColor;

        ColorObjectRecognition();

        void prepareObjectsForRecognition(const ObjectSet& definedObjects);

        template< typename Snapshot >
        ObjectSet recognizeObjects(const Snapshot& snapshot);

        OutputImage image();

    private:
        template< typename Snapshot >
        void createBlocks_(Snapshot snapshot, std::size_t row, AreaBlockList &currentRowBlocks);

        void mergeBlocks_(const AreaBlockList& lastRow, AreaBlockList& currentRow);

        ObjectList objects_;
        ColorMap yMap_;
        ColorMap cbMap_;
        ColorMap crMap_;

        AreaList areas_;

        OutputImage image_;
    };
} // namespace Gecon

#include "ColorObjectRecognition.tpp"

#endif // GECON_COLOROBJECTRECOGNITION_HPP
