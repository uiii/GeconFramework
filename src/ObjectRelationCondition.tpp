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

#include "ObjectRelationCondition.hpp"

namespace Gecon
{
    template< typename Object, typename PropertyType >
    ObjectRelationCondition<Object, PropertyType>::ObjectRelationCondition(
            Object *left,
            Object *right,
            typename ObjectRelationCondition<Object, PropertyType>::Property leftProperty,
            typename ObjectRelationCondition<Object, PropertyType>::Property rightProperty,
            typename ObjectRelationCondition<Object, PropertyType>::Condition condition
    ):
        left_(left),
        right_(right),
        leftProperty_(leftProperty),
        rightProperty_(rightProperty),
        condition_(condition),
        relationEnterEvent_(left, right),
        relationLeaveEvent_(left, right)
    {
    }

    template< typename Object, typename PropertyType >
    typename Gesture<Object, PropertyType>::ObjectSet ObjectRelationCondition<Object, PropertyType>::objects() const
    {
        return { left_, right_ };
    }

    template< typename Object, typename PropertyType >
    const std::string& ObjectRelationCondition<Object, PropertyType>::description() const
    {
        return description_;
    }

    template< typename Object, typename PropertyType >
    const ObjectRelationCondition<Object, PropertyType>::Event& ObjectRelationCondition<Object, PropertyType>::relationEnterEvent() const
    {
        return relationEnterEvent_;
    }

    template< typename Object, typename PropertyType >
    const ObjectRelationCondition<Object, PropertyType>::Event& ObjectRelationCondition<Object, PropertyType>::relationLeaveEvent() const
    {
        return relationLeaveEvent_;
    }

    template< typename Object, typename PropertyType >
    bool ObjectRelationCondition<Object, PropertyType>::check() const
    {
        if(leftProperty_ != &Object::isVisible && left_->isVisible() == false)
        {
            return false;
        }

        if(rightProperty_ != &Object::isVisible && right_->isVisible() == false)
        {
            return false;
        }

        return condition_(*left_, *right_);
    }

    template< typename Object, typename PropertyType >
    typename ObjectRelationCondition<Object, PropertyType>::Ptr makeGestureRelationCondition(
            Object* left,
            Object* right,
            typename ObjectRelationCondition<Object, PropertyType>::Property leftProperty,
            typename ObjectRelationCondition<Object, PropertyType>::Property rightProperty,
            typename ObjectRelationCondition<Object, PropertyType>::Condition condition
    )
    {
        return std::make_shared<ObjectRelationCondition<Object, PropertyType>>(left, right, leftProperty, rightProperty, condition);
    }
} // namespace Gecon
