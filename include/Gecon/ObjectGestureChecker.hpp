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

#ifndef GECON_OBJECTGESTURECHECKER_HPP
#define GECON_OBJECTGESTURECHECKER_HPP

#include <set>
#include <string>

#include "ObjectGesture.hpp"

namespace Gecon
{
    /**
     * Represents gesture checking procedure.
     */
    template< typename Object, typename ObjectContainer = std::set<Object*> >
    class ObjectGestureChecker
    {
    public:
        typedef typename ObjectGesture<Object>::Event Event;
        typedef std::set<Event*> Events;

        typedef ObjectContainer Objects;

        virtual ~ObjectGestureChecker() {}

        /**
         * Check gestures related to this checker.
         *
         * @param objects
         *     Visible objects.
         *     Gestures are checked acording to their states.
         *
         * @return
         *     Occured gesture events.
         */
        virtual Events check(const Objects& objects) = 0;

        virtual void clear() = 0;
        virtual void reset() = 0;
    };
} // namespace Gecon

#endif // GECON_OBJECTGESTURECHECKER_HPP
