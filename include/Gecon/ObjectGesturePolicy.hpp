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

#ifndef GECON_OBJECTGESTUREPOLICY_HPP
#define GECON_OBJECTGESTUREPOLICY_HPP

#include <set>
#include <map>

#include "ObjectGestureChecker.hpp"

namespace Gecon
{
    template< typename Object, typename ObjectContainer = std::set<Object*> >
    class ObjectGesturePolicy
    {
    public:
        typedef ObjectGestureChecker<Object, ObjectContainer> GestureChecker;
        typedef std::set<GestureChecker*> GestureCheckers;

        typedef ObjectContainer Objects;
        typedef typename GestureChecker::Event Event;
        typedef typename GestureChecker::Events Events;

        ObjectGesturePolicy();
        ObjectGesturePolicy(const ObjectGesturePolicy<Object, ObjectContainer>& another);

        ObjectGesturePolicy<Object, ObjectContainer>& operator=(const ObjectGesturePolicy<Object, ObjectContainer>& another);

        void prepareGestureCheckers(const GestureCheckers& checkers);
        Events checkGestures(const Objects& objects);

    private:
        GestureCheckers checkers_;
    };
} // namespace Gecon

#include "private/ObjectGesturePolicy.tpp"

#endif // GECON_OBJECTGESTUREPOLICY_HPP
