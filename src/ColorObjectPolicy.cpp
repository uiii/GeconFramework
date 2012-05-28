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

#include "ColorObjectPolicy.hpp"

#include <algorithm>
#include <cmath>

namespace Gecon
{
    config_variable<std::size_t> ColorObjectPolicy::MINIMAL_OBJECT_SIZE_FRACTION = 1000;

    ColorObjectPolicy::ColorObjectPolicy()
    {
    }

    ColorObjectPolicy::ColorObjectPolicy(const ColorObjectPolicy& another)
    {
        prepareObjects(another.objects_);
    }

    ColorObjectPolicy& ColorObjectPolicy::operator =(const ColorObjectPolicy& another)
    {
        prepareObjects(another.objects_);
        return *this;
    }

    void ColorObjectPolicy::prepareObjects(const ColorObjectPolicy::Objects& definedObjects)
    {
        objects_ = definedObjects;
    }

    const ColorObjectPolicy::OutputImage &ColorObjectPolicy::segmentedImage()
    {
        return image_;
    }

    void ColorObjectPolicy::connectBlocks_(const AreaBlockList& lastRowBlocks, AreaBlockList& currentRowBlocks)
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
                    if(! currentBlock->area) // this current block wans't previously connected to any area
                    {
                        // create new area
                        currentBlock->area = createArea_(*currentBlock);
                    }

                    ++currentBlock;
                }
                else // blocks overlaps
                {
                    if(lastBlock->object == currentBlock->object) // blocks belong together
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
                        if(! currentBlock->area) // it may happen when blocks doesn't belong together
                        {
                            currentBlock->area = createArea_(*currentBlock);
                        }

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

    ColorObjectPolicy::Area* ColorObjectPolicy::createArea_(const ColorObjectPolicy::AreaBlock& block)
    {
        Area* area = new Area(block);
        areas_.push_back(area);

        return area;
    }

    void ColorObjectPolicy::selectVisibleObjects_(Objects &visibleObjects, Point snapshotSize)
    {
        // choose biggest areas for objects
        std::map<Object*, Area*> objectAreaMap;
        for(Object* object : objects_)
        {
            objectAreaMap[object] = 0;
        }

        for(Area* area : areas_)
        {
            if(! area->nested())
            {
                Object* object = area->object();
                Area* objectCurrentArea = objectAreaMap[object];

                if(objectCurrentArea == 0 || objectCurrentArea->size() < area->size())
                {
                    objectAreaMap[object] = area;
                }
            }
        }

        auto objectAreaPair = objectAreaMap.begin();
        while(objectAreaPair != objectAreaMap.end())
        {
            Object* object = objectAreaPair->first;
            Area* area = objectAreaPair->second;

            std::size_t minimalSize = snapshotSize.x * snapshotSize.y / MINIMAL_OBJECT_SIZE_FRACTION;
            if(area != 0 && area->size() > minimalSize)
            {
                object->update(area, snapshotSize);
                object->setVisible(true);

                visibleObjects.insert(object);
            }
            else
            {
                object->update(0, snapshotSize);
                object->setVisible(false);
            }

            ++objectAreaPair;
        }
    }
} // namespace Gecon
