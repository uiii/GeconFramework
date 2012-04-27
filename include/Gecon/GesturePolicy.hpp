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

#ifndef GECON_GESTUREPOLICY_HPP
#define GECON_GESTUREPOLICY_HPP

#include <set>
#include <map>

#include "ObjectGesture.hpp"

namespace Gecon
{
    template< typename Object >
    class GesturePolicy
    {
    public:
        typedef ObjectGesture<Object> Gesture;
        typedef typename Gesture::Event Event;

        typedef std::set<Object*> Objects;
        typedef std::set<Gesture*> Gestures;
        typedef std::set<Event*> Events;

        GesturePolicy();

        void prepareGestures(const Gestures& gestures);
        Events checkGestures(const Objects& objects);

    private:
        std::map<Object*, Gestures> objectGestures_;
        Gestures gesturesToCheck_;
    };
} // namespace Gecon

#include "private/GesturePolicy.tpp"

#endif // GECON_GESTUREPOLICY_HPP
