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
    template< typename Object, typename PropertyType >
    ObjectStateGesture<Object, PropertyType>::ObjectStateGesture():
        object_(0),
        property_(0)
    {
    }

    template< typename Object, typename PropertyType >
    ObjectStateGesture<Object, PropertyType>::ObjectStateGesture(
            Object* object,
            typename ObjectStateGesture<Object, PropertyType>::Property property,
            typename ObjectStateGesture<Object, PropertyType>::Condition condition
    ):
        object_(object),
        property_(property),
        condition_(condition),
        inState_(false),
        objectState_(*object_)
    {
    }

    template< typename Object, typename PropertyType >
    const typename ObjectStateGesture<Object, PropertyType>::Event& ObjectStateGesture<Object, PropertyType>::stateEnterEvent() const
    {
        return stateEnterEvent_;
    }

    template< typename Object, typename PropertyType >
    const typename ObjectStateGesture<Object, PropertyType>::Event& ObjectStateGesture<Object, PropertyType>::stateLeaveEvent() const
    {
        return stateLeaveEvent_;
    }

    template< typename Object, typename PropertyType >
    typename ObjectStateGesture<Object, PropertyType>::ObjectSet ObjectStateGesture<Object, PropertyType>::objects() const
    {
        return { object_ };
    }

    template< typename Object, typename PropertyType >
    void ObjectStateGesture<Object, PropertyType>::check()
    {
        objectState_ = *object_;

        if(property_ != &Object::isVisible && objectState_.isVisible() == false)
        {
            return;
        }

        std::cout << "before check" << std::endl;
        if(condition_((objectState_.*(property_))()))
        {
            std::cout << "in state" << std::endl;
            if(! inState_)
            {
                inState_ = true;
                std::cout << "enter event" << std::endl;
                stateEnterEvent_.raise(*this);
            }
        }
        else
        {
            std::cout << "not in state" << std::endl;
            if(inState_)
            {
                inState_ = false;
                std::cout << "leave event" << std::endl;
                stateLeaveEvent_.raise(*this);
            }
        }
    }

    template< typename Object, typename PropertyType >
    bool ObjectStateGesture<Object, PropertyType>::needCheck() const
    {
        return false; // TODO!!!!!
    }

    template< typename Object, typename PropertyType >
    typename ObjectStateGesture<Object, PropertyType>::Ptr makeObjectStateGesture(
            Object* object,
            PropertyType (Object::*property)() const,
            std::function<bool(const PropertyType&)> condition
    )
    {
        return std::make_shared<ObjectStateGesture<Object, PropertyType> >(object, property, condition);
    }
} // namespace Gecon
