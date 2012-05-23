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

#include "../ObjectGesturePolicy.hpp"

namespace Gecon
{
    template< typename Object, typename ObjectContainer >
    ObjectGesturePolicy<Object, ObjectContainer>::ObjectGesturePolicy()
    {
    }

    template< typename Object, typename ObjectContainer >
    ObjectGesturePolicy<Object, ObjectContainer>::ObjectGesturePolicy(const ObjectGesturePolicy<Object, ObjectContainer>& another)
    {
        prepareGestureCheckers(another.checkers_);
    }

    template< typename Object, typename ObjectContainer >
    ObjectGesturePolicy<Object, ObjectContainer>& ObjectGesturePolicy<Object, ObjectContainer>::operator =(const ObjectGesturePolicy<Object, ObjectContainer>& another)
    {
        prepareGestureCheckers(another.checkers_);
        return *this;
    }

    template< typename Object, typename ObjectContainer >
    void ObjectGesturePolicy<Object, ObjectContainer>::prepareGestureCheckers(const ObjectGesturePolicy<Object, ObjectContainer>::GestureCheckers& checkers)
    {
        checkers_ = checkers;

        for(GestureChecker* checker : checkers_)
        {
            checker->reset();
        }
    }

    template< typename Object, typename ObjectContainer >
    typename ObjectGesturePolicy<Object, ObjectContainer>::Events ObjectGesturePolicy<Object, ObjectContainer>::checkGestures(const ObjectGesturePolicy<Object, ObjectContainer>::Objects& objects)
    {
        Events events;

        for(GestureChecker* checker : checkers_)
        {
            Events checkerEvents = checker->check(objects);
            events.insert(checkerEvents.begin(), checkerEvents.end());
        }

        return events;
    }
} // namespace Gecon
