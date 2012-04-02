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

#include "../ColorArea.hpp"

#include <algorithm>

namespace Gecon
{
    template< typename ColorSpace >
    ColorArea<ColorSpace>::ColorArea(const Block &block):
        object_(0),
        parentArea_(0),
        unionRank_(0),
        size_(0)
    {
        addBlock(block);
    }

    template< typename ColorSpace >
    typename ColorArea<ColorSpace>::ObjectPtr ColorArea<ColorSpace>::object() const
    {
        return object_;
    }

    template< typename ColorSpace >
    void ColorArea<ColorSpace>::addBlock(Block block)
    {
        block.area = this;

        object_ = block.object;

        if(blocks_.empty() || blocks_.back().row != block.row)
        {
            size_ += block.end - block.begin;
            blocks_.push_back(block);
        }
        else
        {
            size_ += block.end - blocks_.back().end;
            blocks_.back().end = block.end;
        }
    }

    template< typename ColorSpace >
    bool ColorArea<ColorSpace>::nested() const
    {
        return parentArea_ != 0;
    }

    template< typename ColorSpace >
    std::size_t ColorArea<ColorSpace>::size()
    {
        includeSubAreas_();

        return size_;
    }

    template< typename ColorSpace >
    const typename ColorArea<ColorSpace>::BlockList& ColorArea<ColorSpace>::blocks()
    {
        includeSubAreas_();

        return blocks_;
    }

    template< typename ColorSpace >
    void ColorArea<ColorSpace>::includeSubAreas_()
    {
        auto rowComparison = [](const Block& left, const Block& right) -> bool { return left.row > right.row; };

        AreaBlockHeap blockHeap;
        makeBlockHeap_(blockHeap, rowComparison);

        BlockList newBlocks;
        std::size_t newSize = 0;

        while(! blockHeap.empty())
        {
            Block newBlock = blockHeap.front(); // currentBlock with minimal row in the heap

            if(newBlocks.empty() || newBlocks.back().row < newBlock.row)
            {
                newBlock.area = this;

                newBlocks.push_back(newBlock);
                newSize += newBlock.end - newBlock.begin;
            }
            else // blocks' rows are equal
            {
                Block& lastAddedBlock = newBlocks.back();

                std::size_t newBegin = std::min(lastAddedBlock.begin, newBlock.begin);
                std::size_t newEnd = std::max(lastAddedBlock.end, newBlock.end);

                newSize += lastAddedBlock.begin - newBegin;
                newSize += newEnd - lastAddedBlock.end;

                lastAddedBlock.begin = newBegin;
                lastAddedBlock.end = newEnd;
            }

            popBlockHeap_(blockHeap, rowComparison);
        }

        std::swap(blocks_, newBlocks);
        size_ = newSize;
    }

    template< typename ColorSpace >
    void ColorArea<ColorSpace>::makeBlockHeap_(ColorArea<ColorSpace>::AreaBlockHeap &blockHeap, const ColorArea<ColorSpace>::AreaBlockComparison& comparison)
    {
        blockHeap.reserve(subAreas_.size() + 1); // allocate place for every subarea and one for this area

        for(AreaPtr subArea : subAreas_)
        {
            subArea->includeSubAreas_();

            if(! subArea->blocks_.empty())
            {
                blockHeap.push_back(subArea->blocks_.front());
                subArea->blocks_.pop_front();
            }
        }

        if(! blocks_.empty())
        {
            blockHeap.push_back(blocks_.front());
            blocks_.pop_front();
        }

        std::make_heap(blockHeap.begin(), blockHeap.end(), comparison);
    }

    template< typename ColorSpace >
    void ColorArea<ColorSpace>::popBlockHeap_(ColorArea<ColorSpace>::AreaBlockHeap &blockHeap, const ColorArea<ColorSpace>::AreaBlockComparison& comparison)
    {
        std::pop_heap(blockHeap.begin(), blockHeap.end(), comparison);

        // add subarea's next block to heap if any
        BlockList& areaBlocks = blockHeap.back().area->blocks_;
        if(! areaBlocks.empty())
        {
            blockHeap.back() = areaBlocks.front();
            areaBlocks.pop_front();

            std::push_heap(blockHeap.begin(), blockHeap.end(), comparison);
        }
        else
        {
            blockHeap.pop_back();
        }
    }

    template< typename ColorSpace >
    void joinAreas(ColorArea<ColorSpace>* first, ColorArea<ColorSpace>* second)
    {
        typename ColorArea<ColorSpace>::AreaList compressList;

        // find first's root area
        ColorArea<ColorSpace>* firstRoot = first;
        while(firstRoot->parentArea_)
        {
            compressList.push_back(firstRoot);
            firstRoot = firstRoot->parentArea_;
            assert(firstRoot != firstRoot->parentArea_);
        }

        // compress first's root successors
        while(! compressList.empty()) // TODO
        {
            ColorArea<ColorSpace>* area = compressList.back();
            compressList.pop_back();

            if(area->parentArea_ != firstRoot) // last area in the list is already compressed
            {
                area->parentArea_ = firstRoot;
                firstRoot->subAreas_.push_back(area);
            }
        }

        // find second's root area
        ColorArea<ColorSpace>* secondRoot = second;
        while(secondRoot->parentArea_)
        {
            compressList.push_back(secondRoot);
            secondRoot = secondRoot->parentArea_;
            assert(secondRoot != secondRoot->parentArea_);
        }

        // compress second's root successors
        while(! compressList.empty()) // TODO
        {
            ColorArea<ColorSpace>* area = compressList.back();
            compressList.pop_back();

            if(area->parentArea_ != secondRoot) // last area in the list is already compressed
            {
                area->parentArea_ = secondRoot;
                secondRoot->subAreas_.push_back(area);
            }
        }

        // join roots if needed
        if(firstRoot != secondRoot) // first and second aren't subareas of the same area
        {
            if(firstRoot->unionRank_ < secondRoot->unionRank_)
            {
                secondRoot->subAreas_.push_back(firstRoot);
                firstRoot->parentArea_ = secondRoot;
            }
            else
            {
                firstRoot->subAreas_.push_back(secondRoot);
                secondRoot->parentArea_ = firstRoot;

                if(firstRoot->unionRank_ == secondRoot->unionRank_)
                {
                    ++(firstRoot->unionRank_);
                }
            }
        }
    }
} // namespace Gecon
