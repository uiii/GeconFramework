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

#include "../GesturePolicy.hpp"

namespace Gecon
{
    template< typename Object >
    GesturePolicy<Object>::GesturePolicy()
    {
    }

    template< typename Object >
    void GesturePolicy<Object>::prepareGestures(const GesturePolicy<Object>::Gestures& gestures)
    {
        objectGestures_.clear();
        gesturesToCheck_.clear();

        for(Gesture* gesture : gestures)
        {
            gesture->reset();

            Objects objects = gesture->objects();
            for(Object* object : objects)
            {
                objectGestures_[object].insert(gesture);
            }
        }

        gesturesToCheck_.insert(gestures.begin(), gestures.end()); // check all gestures for the first time
    }

    template< typename Object >
    typename GesturePolicy<Object>::Events GesturePolicy<Object>::checkGestures(const GesturePolicy<Object>::Objects& objects)
    {
        for(Object* object : objects)
        {
            Gestures gestures = objectGestures_[object];
            gesturesToCheck_.insert(gestures.begin(), gestures.end());
        }

        Events events;

        Gestures needCheck;
        for(Gesture* gesture : gesturesToCheck_)
        {
            Events gestureEvents = gesture->check();

            events.insert(gestureEvents.begin(), gestureEvents.end());

            if(gesture->needCheck())
            {
                needCheck.insert(gesture);
            }
        }

        std::swap(gesturesToCheck_, needCheck);

        return events;
    }
} // namespace Gecon
