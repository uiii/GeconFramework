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
        prepareGestures(another.gestures_);
    }

    template< typename Object, typename ObjectContainer >
    ObjectGesturePolicy<Object, ObjectContainer>& ObjectGesturePolicy<Object, ObjectContainer>::operator =(const ObjectGesturePolicy<Object, ObjectContainer>& another)
    {
        prepareGestures(another.gestures_);
        return *this;
    }

    template< typename Object, typename ObjectContainer >
    void ObjectGesturePolicy<Object, ObjectContainer>::prepareGestures(const ObjectGesturePolicy<Object, ObjectContainer>::Gestures& gestures)
    {
        gestures_ = gestures;

        objectGestures_.clear();
        gesturesToCheck_.clear();

        for(Gesture* gesture : gestures_)
        {
            gesture->reset();

            for(Object* object : gesture->objects())
            {
                objectGestures_[object].insert(gesture);
            }
        }

        gesturesToCheck_.insert(gestures_.begin(), gestures_.end()); // check all gestures for the first time
    }

    template< typename Object, typename ObjectContainer >
    typename ObjectGesturePolicy<Object, ObjectContainer>::Events ObjectGesturePolicy<Object, ObjectContainer>::checkGestures(const ObjectGesturePolicy<Object, ObjectContainer>::Objects& objects)
    {
        for(Object* object : objects)
        {
            Gestures gestures = objectGestures_[object];
            gesturesToCheck_.insert(gestures.begin(), gestures.end());
        }

        Events events;

        for(Gesture* gesture : gesturesToCheck_)
        {
            Events gestureEvents = gesture->check();

            events.insert(gestureEvents.begin(), gestureEvents.end());
        }

        Gestures needCheck;
        // "need check" cyckle is separated from
        // "check" cycle, because needCheck()
        // of one gesture may be affected
        // by check() of another one.
        for(Gesture* gesture : gesturesToCheck_)
        {
            if(gesture->needCheck())
            {
                needCheck.insert(gesture);
            }
        }

        std::swap(gesturesToCheck_, needCheck);

        return events;
    }
} // namespace Gecon
