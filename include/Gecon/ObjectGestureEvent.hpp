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

#ifndef GECON_OBJECTGESTUREEVENT_HPP
#define GECON_OBJECTGESTUREEVENT_HPP

#include <functional>
#include <list>

namespace Gecon
{
    template< typename ObjectGesture >
    class ObjectGestureEvent
    {
    public:
        typedef std::function<void(const ObjectGestureEvent<ObjectGesture>*)> Action;
        typedef std::list<Action> ActionList;

        ObjectGestureEvent();
        virtual ~ObjectGestureEvent();

        const ObjectGesture& gesture() const;

        void raise(const ObjectGesture& gesture) const;

        void connect(Action action) const;

    private:
        mutable ObjectGesture* gesture_;

        mutable ActionList actions_;
    };
} // namespace Gecon

#include "private/ObjectGestureEvent.tpp"

#endif // GECON_OBJECTGESTUREEVENT_HPP
