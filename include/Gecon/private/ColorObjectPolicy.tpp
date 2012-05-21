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

#include "../ColorObjectPolicy.hpp"

#include <map>

namespace Gecon
{
    template< typename Snapshot >
    ColorObjectPolicy::Objects ColorObjectPolicy::recognizeObjects(const Snapshot &snapshot)
    {
        image_.reserve(snapshot.width(), snapshot.height());

        AreaBlockList lastRowBlocks;
        AreaBlockList currentRowBlocks;

        // create areas
        for(std::size_t row = 0; row < snapshot.height(); ++row)
        {
            createBlocks_(snapshot, row, currentRowBlocks);
            connectBlocks_(lastRowBlocks, currentRowBlocks);

            std::swap(lastRowBlocks, currentRowBlocks);
        }

        Objects visibleObjects;
        selectVisibleObjects_(visibleObjects, Point(snapshot.width(), snapshot.height()));

        // remove all areas
        while(! areas_.empty())
        {
            delete areas_.front();
            areas_.pop_front();
        }

        return visibleObjects;
    }

    template< typename Snapshot >
    void ColorObjectPolicy::createBlocks_(Snapshot snapshot, std::size_t row, AreaBlockList &currentRowBlocks)
    {
        currentRowBlocks.clear();

        Object* lastObject = 0;

        for(std::size_t column = 0; column < snapshot.width(); ++column)
        {
            Color pixelColor = snapshot.at(column, row);

            Object* object = objects_.find(pixelColor);
            if(object)
            {
                if(object == lastObject)
                {
                    currentRowBlocks.back().end = column + 1;
                }
                else
                {
                    currentRowBlocks.push_back(AreaBlock{0, object, row, column, column + 1});
                }

                lastObject = object;

                //image_.setAt(column, row, Gecon::Color<RGB>({255,255,255}));
                Gecon::Color<YCbCr> col = snapshot.at(column, row);
                col.y = 128;
                image_.setAt(column, row, col);
            }
            else
            {
                lastObject = 0;

                //image_.setAt(column, row, Gecon::Color<RGB>());
                Gecon::Color<YCbCr> col = snapshot.at(column, row);
                col.y = 128;
                image_.setAt(column, row, col);
            }
        }
    }
} // namespace Gecon
