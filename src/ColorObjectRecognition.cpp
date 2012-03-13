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

#include "ColorObjectRecognition.hpp"

#include <algorithm>

#define Y_RANGE 50
#define CB_RANGE 20
#define CR_RANGE 20

namespace Gecon
{
    ColorObjectRecognition::ColorObjectRecognition():
        yMap_(256),
        cbMap_(256),
        crMap_(256)
    {
    }

    void ColorObjectRecognition::prepareObjectsForRecognition(const ColorObjectRecognition::ObjectList& definedObjects)
    {
        std::size_t objectCount = definedObjects.size();

        objects_.assign(definedObjects.begin(), definedObjects.end());

        // TODO magic 255
        yMap_.assign(256, Bitset(objectCount));
        cbMap_.assign(256, Bitset(objectCount));
        crMap_.assign(256, Bitset(objectCount));

        for(std::size_t i = 0; i < objects_.size(); ++i)
        {
            Color objectColor = objects_.at(i)->color();
            Bitset objectBitset(objectCount);
            objectBitset.set(i, true);

            int yMapBegin = objectColor.y - Y_RANGE;
            int yMapEnd = objectColor.y + Y_RANGE;

            if(yMapBegin < 0) yMapBegin = 0;
            if(yMapEnd > 255) yMapEnd = 255;

            int cbMapBegin = objectColor.cb - CB_RANGE;
            int cbMapEnd = objectColor.cb + CB_RANGE;

            if(cbMapBegin < 0) cbMapBegin = 0;
            if(cbMapEnd > 255) cbMapEnd = 255;

            int crMapBegin = objectColor.cr - CR_RANGE;
            int crMapEnd = objectColor.cr + CR_RANGE;

            if(crMapBegin < 0) crMapBegin = 0;
            if(crMapEnd > 255) crMapEnd = 255;

            auto addObject = [&objectBitset](Bitset& set){ set |= objectBitset; };

            std::for_each(yMap_.begin() + yMapBegin, yMap_.begin() + yMapEnd, addObject);
            std::for_each(cbMap_.begin() + cbMapBegin, cbMap_.begin() + cbMapEnd, addObject);
            std::for_each(crMap_.begin() + crMapBegin, crMap_.begin() + crMapEnd, addObject);
        }
    }

    ColorObjectRecognition::OutputImage ColorObjectRecognition::image()
    {
        return image_;
    }

    void ColorObjectRecognition::connectBlocks_(const AreaBlockList& lastRowBlocks, AreaBlockList& currentRowBlocks)
    {
        AreaBlockList::const_iterator lastBlock = lastRowBlocks.begin();
        AreaBlockList::iterator currentBlock = currentRowBlocks.begin();

        // merge last and current row
        while(currentBlock != currentRowBlocks.end() && lastBlock != lastRowBlocks.end())
        {
            if(lastBlock->end < currentBlock->begin) // this last block doesn't overlap any current block
            {
                ++lastBlock;
            }
            else
            {
                if(currentBlock->end < lastBlock->begin) // this current block doesn't overlap any last block
                {
                    if(! currentBlock->area) // this current wans't previously connected to any area
                    {
                        // create new area
                        currentBlock->area = createArea_(*currentBlock);
                    }

                    ++currentBlock;
                }
                else // blocks overlaps
                {
                    if(lastBlock->object == currentBlock->object)
                    {
                        if(currentBlock->area) // this current block is already assigned to some area
                        {
                            // join last block's and current block's areas
                            joinAreas(lastBlock->area, currentBlock->area);
                        }
                        else // this current block isn't assigned to any area
                        {
                            // assign current block to the last block's area
                            lastBlock->area->addBlock(*currentBlock);

                            currentBlock->area = lastBlock->area;
                        }
                    }

                    if(lastBlock->end < currentBlock->end) // this current block maybe overlaps next last block
                    {
                        ++lastBlock;
                    }
                    else // this last block maybe overlaps next current block
                    {
                        ++currentBlock;
                    }
                }
            }
        }

        // create new areas for remaining current blocks
        while(currentBlock != currentRowBlocks.end())
        {
            if(! currentBlock->area) // this current wans't previously connected to any area
            {
                // create new area
                currentBlock->area = createArea_(*currentBlock);
            }

            ++currentBlock;
        }
    }

    ColorObjectRecognition::AreaPtr ColorObjectRecognition::createArea_(const ColorObjectRecognition::AreaBlock& block)
    {
        AreaPtr area = new Area(block);
        areas_.push_back(area);

        return area;
    }
} // namespace Gecon
