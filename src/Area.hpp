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

#ifndef GECON_AREA_HPP
#define GECON_AREA_HPP

#include <vector>
#include <list>

namespace Gecon
{
    template< typename Object >
    class Area; // forward declaration

    template< typename Object >
    struct AreaBlock
    {
        Area<Object>* area;
        Object* object;

        std::size_t row;
        std::size_t begin;
        std::size_t end;
    };

    template< typename Object >
    class Area
    {
        typedef Gecon::AreaBlock<Object> AreaBlock;

        template< typename T >
        friend void joinAreas(Area<T>* first, Area<T>* second);

    public:
        Area(AreaBlock block);

        void addBlock(AreaBlock block);

    private:
        Area* parentArea_;
        std::list<Area*> subAreas_;

        std::size_t unionRank_;

        std::vector<AreaBlock> blocks_;
    };

    template< typename Object >
    void joinAreas(Area<Object>* first, Area<Object>* second);
} // namespace Gecon

#include "Area.tpp"

#endif // GECON_AREA_HPP
