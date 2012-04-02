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

#ifndef GECON_OBJECTSTATECONDITION_HPP
#define GECON_OBJECTSTATECONDITION_HPP

#include "ObjectGesture.hpp"
#include "ObjectGestureEvent.hpp"

#include <functional>

namespace Gecon
{
    template< typename Object, typename PropertyType >
    class ObjectStateCondition : public ObjectGesture<Object>
    {
    public:
        typedef PropertyType (Object::*Property)();
        typedef std::function<bool(const PropertyType&)> Condition;

        typedef ObjectGestureEvent<ObjectStateCondition<Object, PropertyType> > Event;

        ObjectStateCondition(Object* object, Property property, Condition condition, const std::string& description = "");

        ObjectSet objects() const;
        const std::string& description() const;

        const Event& stateEnterEvent() const;
        const Event& stateLeaveEvent() const;

        bool check() const;

    private:
        Object* object_;
        Property property_;
        Condition condition_;

        Event stateEnterEvent_;
        Event stateLeaveEvent_;

    public:
        typedef std::shared_ptr<ObjectStateCondition> Ptr;
    };

    template< typename Object, typename PropertyType >
    typename ObjectStateCondition<Object, PropertyType>::Ptr makeGestureStateCondition(
            Object* object,
            typename ObjectStateCondition<Object, PropertyType>::Property property,
            typename ObjectStateCondition<Object, PropertyType>::Condition condition
    );
} // namespace Gecon

#include "ObjectStateCondition.tpp"

#endif // GECON_OBJECTSTATECONDITION_HPP
