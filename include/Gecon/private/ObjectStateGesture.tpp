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

#include "../ObjectStateGesture.hpp"

#include <iostream>

namespace Gecon
{
    template< typename Object >
    template< typename PropertyType >
    ObjectStateGesture<Object>::ObjectStateGesture(
            Object* object,
            ObjectStateGesture<Object>::Property<PropertyType> property,
            Relation<PropertyType> relation,
            PropertyType value):
        object_(object),
        mustBeVisible_(needVisible<Object>(property)),
        condition_(std::bind(relation, std::bind(property, std::placeholders::_1), value)),
        inState_(false)
    {
    }

    template< typename Object >
    ObjectStateGesture<Object>& ObjectStateGesture<Object>::operator =(const ObjectStateGesture<Object>& another)
    {
        object_ = another.object_;
        mustBeVisible_ = another.mustBeVisible_;
        condition_ = another.condition_;

        reset();

        return *this;
    }

    template< typename Object >
    typename ObjectStateGesture<Object>::Event* ObjectStateGesture<Object>::stateEnterEvent()
    {
        return &stateEnterEvent_;
    }

    template< typename Object >
    typename ObjectStateGesture<Object>::Event* ObjectStateGesture<Object>::stateLeaveEvent()
    {
        return &stateLeaveEvent_;
    }

    template< typename Object >
    Object* ObjectStateGesture<Object>::object() const
    {
        return object_;
    }

    template< typename Object >
    typename ObjectStateGesture<Object>::Events ObjectStateGesture<Object>::check()
    {
        Events events;

        bool correctVisibility = object_->isVisible() || ! mustBeVisible_;

        if(correctVisibility && condition_(*object_))
        {
            if(! inState_)
            {
                inState_ = true;
                events.insert(&stateEnterEvent_);
            }
        }
        else
        {
            if(inState_)
            {
                inState_ = false;
                events.insert(&stateLeaveEvent_);
            }
        }

        return events;
    }

    template< typename Object >
    bool ObjectStateGesture<Object>::needCheck() const
    {
        return inState_ || ! mustBeVisible_;
    }

    template< typename Object >
    void ObjectStateGesture<Object>::reset()
    {
        inState_ = false;
    }
} // namespace Gecon
