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
    ObjectRelationGesture<Object> &ObjectRelationGesture<Object>::operator=(const ObjectRelationGesture<Object>& another)
    {
        left_ = another.left_;
        right_ = another.right_;

        leftMustBeVisible_ = another.leftMustBeVisible_;
        rightMustBeVisible_ = another.rightMustBeVisible_;

        condition_ = another.condition_;

        reset();

        return *this;
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
    Object* ObjectRelationGesture<Object>::leftObject() const
    {
        return left_;
    }

    template< typename Object >
    Object* ObjectRelationGesture<Object>::rightObject() const
    {
        return right_;
    }

    template< typename Object >
    typename ObjectRelationGesture<Object>::Events ObjectRelationGesture<Object>::check()
    {
        Events events;

        bool correctLeftVisibility = left_->isVisible() || ! leftMustBeVisible_;
        bool correctRightVisibility = right_->isVisible() || ! rightMustBeVisible_;

        if(correctLeftVisibility && correctRightVisibility && condition_(*left_, *right_))
        {
            if(! inRelation_)
            {
                inRelation_ = true;
                events.insert(&relationEnterEvent_);
            }
        }
        else
        {
            if(inRelation_)
            {
                inRelation_ = false;
                events.insert(&relationLeaveEvent_);
            }
        }

        return events;
    }

    template< typename Object >
    bool ObjectRelationGesture<Object>::needCheck() const
    {
        return inRelation_ || ! leftMustBeVisible_ || ! rightMustBeVisible_;
    }

    template< typename Object >
    void ObjectRelationGesture<Object>::reset()
    {
        inRelation_ = false;
    }
} // namespace Gecon
