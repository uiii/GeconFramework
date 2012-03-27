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
        typedef Object* ObjectPtr;
        typedef ObjectGesture<Object> Gesture;
        typedef Gesture* GesturePtr;

        typedef std::set<ObjectPtr> ObjectSet;
        typedef std::set<GesturePtr> GestureSet;

        GesturePolicy();

        void prepareGestures(const GestureSet& gestures);
        void checkGestures(const ObjectSet& objects);

    private:
        std::map<ObjectPtr, GestureSet> objectGestures_;
        GestureSet gesturesToCheck_;
    };
} // namespace Gecon

#include "GesturePolicy.tpp"

#endif // GECON_GESTUREPOLICY_HPP
