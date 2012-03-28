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

#include "ObjectStateCondition.hpp"

namespace Gecon
{
    template< typename Object, typename PropertyType >
    ObjectStateCondition<Object, PropertyType>::ObjectStateCondition(
            Object* object,
            typename ObjectStateCondition<Object, PropertyType>::Property property,
            typename ObjectStateCondition<Object, PropertyType>::Condition condition,
            const std::string& description
    ):
        object_(object),
        description_(description),
        property_(property),
        condition_(condition)
    {
    }

    template< typename Object, typename PropertyType >
    Gesture<Object, PropertyType>::ObjectSet ObjectStateCondition<Object>::objects() const
    {
        return { object_ };
    }

    const std::string ObjectStateCondition::description() const
    {
        return description_;
    }

    template< typename Object, typename PropertyType >
    void ObjectStateCondition<Object, PropertyType>::check() const
    {
        if(property_ != &Object::isVisible && object_->isVisible() == false)
        {
            return;
        }

        return condition_(property_(*object_));
    }

    template< typename Object, typename PropertyType >
    typename ObjectStateCondition<Object, PropertyType>::Ptr makeGestureStateCondition(
            Object* object,
            typename ObjectStateCondition<Object, PropertyType>::Property property,
            typename ObjectStateCondition<Object, PropertyType>::Condition condition
    )
    {
        return std::make_shared<ObjectStateCondition<Object, PropertyType>(object, property, condition);
    }
} // namespace Gecon
