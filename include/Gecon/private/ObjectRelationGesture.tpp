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
    typename ObjectGesture<Object>::Objects ObjectRelationGesture<Object>::objects() const
    {
        return { left_, right_ };
    }

    template< typename Object >
    typename ObjectRelationGesture<Object>::Event* ObjectRelationGesture<Object>::relationEnterEvent()
    {
        return &relationEnterEvent_;
    }

    template< typename Object >
    typename ObjectRelationGesture<Object>::Event* ObjectRelationGesture<Object>::relationLeaveEvent()
    {
        return &relationLeaveEvent_;
    }

    template< typename Object >
    typename ObjectRelationGesture<Object>::Event* ObjectRelationGesture<Object>::inRelationEvent()
    {
        return &inRelationEvent_;
    }

    template< typename Object >
    typename ObjectRelationGesture<Object>::Event* ObjectRelationGesture<Object>::notInRelationEvent()
    {
        return &notInRelationEvent_;
    }

    template< typename Object >
    typename ObjectRelationGesture<Object>::Events ObjectRelationGesture<Object>::check()
    {
        Events events;

        leftState_ = *left_;
        rightState_ = *right_;

        bool correctLeftVisibility = leftState_.isVisible() || ! leftMustBeVisible_;
        bool correctRightVisibility = rightState_.isVisible() || ! rightMustBeVisible_;

        if(correctLeftVisibility && correctRightVisibility && condition_(leftState_, rightState_))
        {
            if(! inRelation_)
            {
                inRelation_ = true;
                std::cout << "enter event" << std::endl;
                events.insert(&relationEnterEvent_);
            }

            std::cout << "in relation" << std::endl;
            events.insert(&inRelationEvent_);
        }
        else
        {
            if(inRelation_)
            {
                inRelation_ = false;
                std::cout << "leave event" << std::endl;
                events.insert(&relationLeaveEvent_);
            }

            std::cout << "not in relation" << std::endl;
            events.insert(&notInRelationEvent_);
        }

        return events;
    }

    template< typename Object >
    bool ObjectRelationGesture<Object>::needCheck() const
    {
        // TODO
        //return inRelation_ || ! leftMustBeVisible_ || ! rightMustBeVisible_;
        return true;
    }

    template< typename Object >
    void ObjectRelationGesture<Object>::reset()
    {
        inRelation_ = false;
    }
} // namespace Gecon
