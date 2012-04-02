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

#ifndef GECON_OBJECTRELATIONCONDITION_HPP
#define GECON_OBJECTRELATIONCONDITION_HPP

#include "ObjectGesture.hpp"
#include "ObjectGestureEvent.hpp"

#include <functional>

namespace Gecon
{
    template< typename Object, typename PropertyType >
    class ObjectRelationCondition : public ObjectGesture<Object>
    {
    public:
        typedef std::function<PropertyType(const Object&)> Property;
        typedef std::function<bool(const PropertyType&, const PropertyType&)> Condition;

        typedef ObjectGestureEvent<ObjectRelationCondition<Object, PropertyType> > Event;

        ObjectRelationCondition(
                Object* left,
                Object* right,
                Property leftProperty,
                Property rightProperty,
                Condition condition,
                const std::string& description = ""
        );

        ObjectSet objects() const;
        const std::string& description() const;

        const Event& relationEnterEvent() const;
        const Event& relationLeaveEvent() const;

        bool check() const;

    private:
        Object* left_;
        Object* right_;

        Property leftProperty_;
        Property rightProperty_;
        Condition condition_;

        std::string description_;

        Event relationEnterEvent_;
        Event relationLeaveEvent_;

    public:
        typedef std::shared_ptr<ObjectRelationCondition<Object> > Ptr;
    };

    template< typename Object, typename PropertyType >
    typename ObjectRelationCondition<Object, PropertyType>::Ptr makeGestureRelationCondition(
            Object* left,
            Object* right,
            typename ObjectRelationCondition<Object, PropertyType>::Property leftProperty,
            typename ObjectRelationCondition<Object, PropertyType>::Property rightProperty,
            typename ObjectRelationCondition<Object, PropertyType>::Condition condition
    );
} // namespace Gecon

#include "ObjectRelationCondition.tpp"

#endif // GECON_OBJECTRELATIONCONDITION_HPP
