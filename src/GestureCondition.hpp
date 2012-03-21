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

#ifndef GECON_GESTURECONDITION_HPP
#define GECON_GESTURECONDITION_HPP

namespace Gecon
{
    template< typename Object >
    class GestureCondition
    {
    public:
        typedef std::vector<Object*> ObjectList;

        virtual ~GestureCondition() {}

        virtual ObjectList objects() const = 0;

        virtual bool check() const = 0;
    };
} // namespace Gecon

#endif // GECON_GESTURECONDITION_HPP
