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

#ifndef GECON_COLORAREA_HPP
#define GECON_COLORAREA_HPP

#include <vector>
#include <list>

#include "Image.hpp" // TODO remove

namespace Gecon
{
    template< typename ColorSpace >
    class ColorObject; // forward declaration

    template< typename ColorSpace >
    class ColorArea
    {
        template< typename T >
        friend void joinAreas(ColorArea<T>* first, ColorArea<T>* second);

    public:
        typedef ColorObject<ColorSpace> Object;
        typedef Object* ObjectPtr;

        typedef ColorArea<ColorSpace> Area;
        typedef Area* AreaPtr;
        typedef const Area* AreaConstPtr;

        struct Block
        {
            AreaPtr area;
            //Color<ColorSpace> color;
            ObjectPtr object;

            std::size_t row;
            std::size_t begin;
            std::size_t end;
        };

        typedef std::list<Block> BlockList;

        ColorArea(const Block& block);

        ObjectPtr object() const;

        void addBlock(Block block);
        const BlockList& blocks();

        bool nested() const;
        std::size_t size();

    private:
        typedef std::list<AreaPtr> AreaList;

        typedef std::vector<Block> AreaBlockHeap;
        typedef bool (*AreaBlockComparison)(const Block& left, const Block& right);

        void includeSubAreas_();

        void makeBlockHeap_(AreaBlockHeap& blockHeap, const AreaBlockComparison& comparison);
        void popBlockHeap_(AreaBlockHeap& blockHeap, const AreaBlockComparison& comparison);

        ObjectPtr object_;

        AreaPtr parentArea_;
        AreaList subAreas_;

        std::size_t unionRank_;

        BlockList blocks_;
        std::size_t size_;
    };

    template< typename Object >
    void joinAreas(ColorArea<Object>* first, ColorArea<Object>* second);
} // namespace Gecon

#include "private/ColorArea.tpp"

#endif // GECON_COLORAREA_HPP
