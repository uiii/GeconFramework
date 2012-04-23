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
    /*template< typename Object >
    ObjectStateGesture<Object>::ObjectStateGesture():
        object_(0),
        inState_(false)
    {
    }*/

    template< typename Object >
    template< typename PropertyType >
    ObjectStateGesture<Object>::ObjectStateGesture(
            Object* object,
            ObjectStateGesture<Object>::Property<PropertyType> property,
            Relation<PropertyType> relation,
            PropertyType value):
        object_(object),
        objectState_(*object_),
        mustBeVisible_(needVisible<Object>(property)),
        condition_(std::bind(relation, std::bind(property, std::placeholders::_1), value)),
        inState_(false)
    {
    }

    template< typename Object >
    Event* ObjectStateGesture<Object>::stateEnterEvent()
    {
        return &stateEnterEvent_;
    }

    template< typename Object >
    Event* ObjectStateGesture<Object>::stateLeaveEvent()
    {
        return &stateLeaveEvent_;
    }

    template< typename Object >
    typename ObjectStateGesture<Object>::ObjectSet ObjectStateGesture<Object>::objects() const
    {
        return { object_ };
    }

    template< typename Object >
    void ObjectStateGesture<Object>::check()
    {
        objectState_ = *object_;

        if(mustBeVisible_ && objectState_.isVisible() == false)
        {
            return;
        }

        std::cout << "before check" << std::endl;
        if(condition_(objectState_))
        {
            std::cout << "in state" << std::endl;
            if(! inState_)
            {
                inState_ = true;
                std::cout << "enter event" << std::endl;
                stateEnterEvent_.raise();
            }
        }
        else
        {
            std::cout << "not in state" << std::endl;
            if(inState_)
            {
                inState_ = false;
                std::cout << "leave event" << std::endl;
                stateLeaveEvent_.raise();
            }
        }
    }

    template< typename Object >
    bool ObjectStateGesture<Object>::needCheck() const
    {
        return inState_ || ! mustBeVisible_;
    }
} // namespace Gecon
