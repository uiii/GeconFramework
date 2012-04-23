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

#include "../ObjectRelationGesture.hpp"

#include <iostream>
#include <memory>

namespace Gecon
{
    template< typename Object >
    template< typename PropertyType >
    ObjectRelationGesture<Object>::ObjectRelationGesture(
            Object* left,
            ObjectRelationGesture<Object>::Property<PropertyType> leftProperty,
            ObjectRelationGesture<Object>::Relation<PropertyType> relation,
            Object* right,
            ObjectRelationGesture<Object>::Property<PropertyType> rightProperty):
        left_(left),
        right_(right),
        leftState_(*left_),
        rightState_(*right_),
        leftMustBeVisible_(needVisible<Object>(leftProperty)),
        rightMustBeVisible_(needVisible<Object>(rightProperty)),
        condition_(
            std::bind(
                relation,
                std::bind(leftProperty, std::placeholders::_1),
                std::bind(rightProperty, std::placeholders::_2)
            )
        ),
        inRelation_(false)
    {
    }


    template< typename Object >
    typename ObjectGesture<Object>::ObjectSet ObjectRelationGesture<Object>::objects() const
    {
        return { left_, right_ };
    }

    template< typename Object >
    Event* ObjectRelationGesture<Object>::relationEnterEvent()
    {
        return &relationEnterEvent_;
    }

    template< typename Object >
    Event* ObjectRelationGesture<Object>::relationLeaveEvent()
    {
        return &relationLeaveEvent_;
    }

    template< typename Object >
    void ObjectRelationGesture<Object>::check()
    {
        leftState_ = *left_;
        rightState_ = *right_;

        if(leftMustBeVisible_ && leftState_.isVisible() == false)
        {
            return;
        }

        if(rightMustBeVisible_ && rightState_.isVisible() == false)
        {
            return;
        }

        if(condition_(leftState_, rightState_))
        {
            std::cout << "in relation" << std::endl;
            if(! inRelation_)
            {
                inRelation_ = true;
                std::cout << "enter event" << std::endl;
                relationEnterEvent_.raise();
            }
        }
        else
        {
            std::cout << "not in relation" << std::endl;
            if(inRelation_)
            {
                inRelation_ = false;
                std::cout << "leave event" << std::endl;
                relationLeaveEvent_.raise();
            }
        }
    }

    template< typename Object >
    bool ObjectRelationGesture<Object>::needCheck() const
    {
        //return inRelation_ || ! leftMustBeVisible_ || ! rightMustBeVisible_;
        return true;
    }
} // namespace Gecon
